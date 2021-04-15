#ifndef PAINT_INC_COMMAND_H_
#define PAINT_INC_COMMAND_H_

#include <memory>
#include <filesystem>

#include "image.h"
#include "rotation.h"

namespace paint
{
    class Command
    {
    public:
        explicit Command(std::string name) : command_name_(name){};
        virtual ~Command(){};

        virtual void Invoke(AbstractImage &im) = 0;

        std::string CommandName() const { return command_name_; };

    private:
        std::string command_name_;
    };

    class SaveCommand : public Command
    {
    public:
        explicit SaveCommand(std::filesystem::path path_to_file) : Command("SaveCommand"), path_to_file_(path_to_file){};
        virtual ~SaveCommand(){};

        virtual void Invoke([[maybe_unused]] AbstractImage &im) override
        {
            std::cout << "TODO: " << this->CommandName() << "::Invoke\n";
        };

    private:
        std::filesystem::path path_to_file_;
    };

    class ColorCommand : public Command
    {
    public:
        explicit ColorCommand(std::shared_ptr<Color> &&color) : Command("ColorCommand"), color_(color){};
        virtual ~ColorCommand(){};

        virtual void Invoke(AbstractImage &im) override
        {
            std::cout << "TODO: " << this->CommandName() << "::Invoke\n";
            im.SetNextColor(color_);
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

        virtual void Invoke(AbstractImage &im) override
        {
            std::cout << "TODO: " << this->CommandName() << "::Invoke\n";
            im.DrawLine();
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

        virtual void Invoke(AbstractImage &im) override
        {
            std::cout << "TODO: " << this->CommandName() << "::Invoke\n";
            im.DrawCircle();
        };

        void SetCircleFill(bool do_fill) { fill_ = do_fill; };

        void AddCircleFillColor(std::shared_ptr<Color> &&color) { fill_color_ = std::move(color); };
        void AddCircleFillColor(const Color &color) { fill_color_->SetColor(color); };

        void AddCircleBorderColor(std::shared_ptr<Color> &&color) { border_color_ = std::move(color); };
        void AddCircleBorderColor(const Color &color) { border_color_->SetColor(color); };

        void AddCircleBorderWidth(const Unit &border_width)
        {
            if (border_width > radius_)
            {
                throw "border_width > radius_";
            }

            border_width_ = border_width;
        };

    private:
        std::shared_ptr<BasePoint> center_;
        Unit radius_;

        // Optional parameters
        bool fill_ = false; // Set fill_ to false as default
        std::shared_ptr<Color> fill_color_;
        std::shared_ptr<Color> border_color_;
        Unit border_width_;
    };

    class BucketCommand : public Command
    {
    public:
        explicit BucketCommand(std::shared_ptr<BasePoint> &&point) : Command("BucketCommand"), point_{std::move(point)} {};
        virtual ~BucketCommand(){};

        virtual void Invoke(AbstractImage &im) override
        {
            std::cout << "TODO: " << this->CommandName() << "::Invoke\n";
            im.DrawBucket();
        };

        void AddFillColor(std::shared_ptr<Color> &&color) { fill_color_ = std::move(color); };

    private:
        std::shared_ptr<BasePoint> point_;

        // Optional parameters
        std::shared_ptr<Color> fill_color_;
    };

    class ResizeCommand : public Command
    {
    public:
        explicit ResizeCommand(std::shared_ptr<BasePoint> &&new_size) : Command("ResizeCommand"), new_size_(std::move(new_size)){};
        virtual ~ResizeCommand(){};

        virtual void Invoke(AbstractImage &im) override
        {
            std::cout << "TODO: " << this->CommandName() << "::Invoke\n";
            im.Resize();
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

        virtual void Invoke(AbstractImage &im) override
        {
            std::cout << "TODO: " << this->CommandName() << "::Invoke\n";
            im.Rotate();
        };

    private:
        Rotation rotation_;
    };

    class InvertColorCommand : public Command
    {
    public:
        explicit InvertColorCommand() : Command("InvertColorCommand"){};
        virtual ~InvertColorCommand(){};

        virtual void Invoke(AbstractImage &im) override
        {
            std::cout << "TODO: " << this->CommandName() << "::Invoke\n";
            im.InvertColors();
        };
    };

    class GrayscaleCommand : public Command
    {
    public:
        explicit GrayscaleCommand() : Command("GrayscaleCommand"){};
        virtual ~GrayscaleCommand(){};

        virtual void Invoke(AbstractImage &im) override
        {
            std::cout << "TODO: " << this->CommandName() << "::Invoke\n";
            im.ConvertToGrayscale();
        };
    };

    class CropCommand : public Command
    {
    public:
        explicit CropCommand(std::shared_ptr<BasePoint> &&point1, std::shared_ptr<BasePoint> &&point2) : Command("CropCommand"),
                                                                                                         point1_{std::move(point1)},
                                                                                                         point2_{std::move(point2)} {};
        virtual ~CropCommand(){};

        virtual void Invoke(AbstractImage &im) override
        {
            std::cout << "TODO: " << this->CommandName() << "::Invoke\n";
            im.Crop();
        };

    private:
        std::shared_ptr<BasePoint> point1_;
        std::shared_ptr<BasePoint> point2_;
    };

    class UndoCommand : public Command
    {
    public:
        explicit UndoCommand() : Command("UndoCommand"){};
        virtual ~UndoCommand(){};

        virtual void Invoke(AbstractImage &im) override
        {
            std::cout << "TODO: " << this->CommandName() << "::Invoke\n";
            im.Undo();
        };
    };

    class RedoCommand : public Command
    {
    public:
        explicit RedoCommand() : Command("RedoCommand"){};
        virtual ~RedoCommand(){};

        virtual void Invoke(AbstractImage &im) override
        {
            std::cout << "TODO: " << this->CommandName() << "::Invoke\n";
            im.Redo();
        };
    };
}

#endif // PAINT_INC_COMMAND_H_