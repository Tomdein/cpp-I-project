#ifndef PAINT_INC_COMMAND_H_
#define PAINT_INC_COMMAND_H_

#include <memory>
#include <optional>
#include <filesystem>

#include "unit.h"
#include "point.h"
#include "color.h"
#include "image.h"
#include "rotation.h"

namespace paint
{
    class Command
    {
    public:
        explicit Command(std::string name) : command_name_(name){};
        virtual ~Command(){};

        virtual void Invoke(Image &im) = 0;

        std::string CommandName() const { return command_name_; };

    private:
        std::string command_name_;
    };

    class LoadCommand : public Command
    {
    public:
        explicit LoadCommand(std::filesystem::path path_to_file) : Command("LoadCommand"), path_to_file_(path_to_file){};
        virtual ~LoadCommand(){};

        virtual void Invoke([[maybe_unused]] Image &im) override
        {
            std::cerr << "LoadCommand should not be invoked! " << this->CommandName() << "::Invoke\n";
        };

        std::filesystem::path FilePath() const
        {
            return path_to_file_;
        }

    private:
        std::filesystem::path path_to_file_;
    };

    class SaveCommand : public Command
    {
    public:
        explicit SaveCommand(std::filesystem::path path_to_file) : Command("SaveCommand"), path_to_file_(path_to_file){};
        virtual ~SaveCommand(){};

        virtual void Invoke(Image &im) override
        {
            if (path_to_file_.empty())
            {
                im.SaveImage();
            }
            else
            {
                im.SaveImage(path_to_file_);
            }
        };

    private:
        std::filesystem::path path_to_file_;
    };

    class ColorCommand : public Command
    {
    public:
        explicit ColorCommand(std::shared_ptr<Color> &&color) : Command("ColorCommand"), color_(color){};
        virtual ~ColorCommand(){};

        virtual void Invoke(Image &im) override
        {
            im.painter.SetNextColor(color_);
        };

    private:
        std::shared_ptr<Color> color_;
    };

    class LineCommand : public Command
    {
    public:
        explicit LineCommand(std::shared_ptr<BasePoint> &&start, std::shared_ptr<BasePoint> &&end) : Command("LineCommand"),
                                                                                                     start_point_(std::move(start)),
                                                                                                     end_point_(std::move(end)){};
        virtual ~LineCommand(){};

        virtual void Invoke(Image &im) override
        {
            im.painter.DrawLine(*start_point_, *end_point_, line_color_, line_width_);
        };

        void AddLineColor(std::shared_ptr<Color> &&color) { line_color_.emplace(std::move(color)); };
        void AddLineWidth(Unit color) { line_width_.emplace(color); };

    private:
        std::shared_ptr<BasePoint> start_point_;
        std::shared_ptr<BasePoint> end_point_;

        // Optional parameters
        std::optional<std::shared_ptr<Color>> line_color_;
        std::optional<Unit> line_width_;
    };

    class CircleCommand : public Command
    {
    public:
        explicit CircleCommand(std::shared_ptr<BasePoint> &&center, const Unit &radius) : Command("CircleCommand"), center_{std::move(center)}, radius_{radius} {};
        virtual ~CircleCommand(){};

        virtual void Invoke(Image &im) override
        {
            im.painter.DrawCircle(*center_, radius_, fill_.value_or(false), fill_color_, border_color_, border_width_);
        };

        void SetCircleFill(bool do_fill) { fill_ = do_fill; };

        void AddCircleFillColor(std::shared_ptr<Color> &&color) { fill_color_ = std::make_optional<std::shared_ptr<Color>>(std::move(color)); };
        void AddCircleFillColor(const Color &color) { fill_color_ = std::make_optional<std::shared_ptr<Color>>(color.clone()); };

        void AddCircleBorderColor(std::shared_ptr<Color> &&color) { border_color_ = std::make_optional<std::shared_ptr<Color>>(std::move(color)); };
        void AddCircleBorderColor(const Color &color) { border_color_ = std::make_optional<std::shared_ptr<Color>>(color.clone()); };

        void AddCircleBorderWidth(const Unit &border_width)
        {
            if (border_width > radius_)
            {
                std::cerr << "CircleCommand - border_width > radius_!" << std::endl;
                throw "border_width > radius_";
            }

            border_width_ = border_width;
        };

    private:
        std::shared_ptr<BasePoint> center_;
        Unit radius_;

        // Optional parameters
        std::optional<bool> fill_ = false; // Set fill_ to false as default
        std::optional<std::shared_ptr<Color>> fill_color_;
        std::optional<std::shared_ptr<Color>> border_color_;
        std::optional<Unit> border_width_;
    };

    class BucketCommand : public Command
    {
    public:
        explicit BucketCommand(std::shared_ptr<BasePoint> &&point) : Command("BucketCommand"), point_{std::move(point)} {};
        virtual ~BucketCommand(){};

        virtual void Invoke(Image &im) override
        {
            im.painter.DrawBucket(*point_, fill_color_);
        };

        void AddFillColor(std::shared_ptr<Color> &&color) { fill_color_ = std::make_optional<std::shared_ptr<Color>>(std::move(color)); };
        void AddFillColor(const Color &color) { fill_color_ = std::make_optional<std::shared_ptr<Color>>(color.clone()); };

    private:
        std::shared_ptr<BasePoint> point_;

        // Optional parameters
        std::optional<std::shared_ptr<Color>> fill_color_;
    };

    class CropCommand : public Command
    {
    public:
        explicit CropCommand(std::shared_ptr<BasePoint> &&point1, std::shared_ptr<BasePoint> &&point2) : Command("CropCommand"),
                                                                                                         point1_{std::move(point1)},
                                                                                                         point2_{std::move(point2)} {};
        virtual ~CropCommand(){};

        virtual void Invoke(Image &im) override
        {
            im.painter.Crop(*point1_, *point2_);
        };

    private:
        std::shared_ptr<BasePoint> point1_;
        std::shared_ptr<BasePoint> point2_;
    };

    class ResizeCommand : public Command
    {
    public:
        explicit ResizeCommand(std::shared_ptr<BasePoint> &&new_size) : Command("ResizeCommand"), new_size_(std::move(new_size)){};
        virtual ~ResizeCommand(){};

        virtual void Invoke(Image &im) override
        {
            im.painter.Resize(*new_size_);
        };

    private:
        std::shared_ptr<BasePoint> new_size_;
    };

    // TODO RotateCommand
    class RotateCommand : public Command
    {
    public:
        explicit RotateCommand(const Rotation &rotation) : Command("RotateCommand"), rotation_(rotation){};
        virtual ~RotateCommand(){};

        virtual void Invoke(Image &im) override
        {
            im.painter.Rotate(rotation_);
        };

    private:
        Rotation rotation_;
    };

    class InvertColorCommand : public Command
    {
    public:
        explicit InvertColorCommand() : Command("InvertColorCommand"){};
        virtual ~InvertColorCommand(){};

        virtual void Invoke(Image &im) override
        {
            im.painter.InvertColors();
        };
    };

    class GrayscaleCommand : public Command
    {
    public:
        explicit GrayscaleCommand() : Command("GrayscaleCommand"){};
        virtual ~GrayscaleCommand(){};

        virtual void Invoke(Image &im) override
        {
            im.painter.ConvertToGrayscale();
        };
    };

    class UndoCommand : public Command
    {
    public:
        explicit UndoCommand() : Command("UndoCommand"){};
        virtual ~UndoCommand(){};

        virtual void Invoke(Image &im) override
        {
            im.Undo();
        };
    };

    class RedoCommand : public Command
    {
    public:
        explicit RedoCommand() : Command("RedoCommand"){};
        virtual ~RedoCommand(){};

        virtual void Invoke(Image &im) override
        {
            im.Redo();
        };
    };
}

#endif // PAINT_INC_COMMAND_H_