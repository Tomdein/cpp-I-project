#ifndef PAINT_INC_COMMAND_H_
#define PAINT_INC_COMMAND_H_

#include <memory>

#include "image.h"

namespace paint
{
    class Command
    {
    public:
        explicit Command(std::string name) : command_name_(name){};

        virtual void Invoke(AbstractImage &im);

        std::string CommandName() const;

    private:
        std::string command_name_;
    };

    enum class Rotation
    {
        kClock,
        kCounterClock,
    };

    class ColorCommand : public Command
    {
    public:
        explicit ColorCommand(std::shared_ptr<paint::Color> &&color) : Command("ColorCommand"), color_(color){};

        virtual void Invoke(AbstractImage &im) override
        {
            im.SetColor(color_);
        };

    private:
        std::shared_ptr<Color> color_;
    };

    class LineCommand : public Command
    {
    public:
        explicit LineCommand(std::string name) : Command(name){};

        virtual void Invoke(AbstractImage &im) override { im.DrawLine(); };

    private:
        std::shared_ptr<BasePoint> start_point_;
        std::shared_ptr<BasePoint> end_point_;

        // Optional parameters
        std::shared_ptr<Color> line_color_;
        std::shared_ptr<BaseUnit> line_width_;
    };

    class CircleCommand : public Command
    {
    public:
        explicit CircleCommand(std::string name) : Command(name){};

        virtual void Invoke(AbstractImage &im) override { im.DrawCircle(); };

    private:
        std::shared_ptr<BasePoint> center_;
        std::shared_ptr<BaseUnit> raduis_;

        // Optional parameters
        bool fill_ = false; // Set fill_ to false as default
        std::shared_ptr<Color> fill_color_;
        std::shared_ptr<Color> border_color_;
        std::shared_ptr<BaseUnit> border_width_;
    };

    class BucketCommand : public Command
    {
    public:
        explicit BucketCommand(std::string name) : Command(name){};

        virtual void Invoke(AbstractImage &im) override { im.DrawBucket(); };

    private:
        std::shared_ptr<BasePoint> point_;

        // Optional parameters
        std::shared_ptr<Color> fill_color_;
    };

    class ResizeCommand : public Command
    {
    public:
        explicit ResizeCommand(std::string name) : Command(name){};

        virtual void Invoke(AbstractImage &im) override { im.Resize(); };

    private:
        std::shared_ptr<BasePoint> size_;
    };

    // TODO RotateCommand
    class RotateCommand : public Command
    {
    public:
        explicit RotateCommand(std::string name) : Command(name){};

        virtual void Invoke(AbstractImage &im) override { im.Rotate(); };

    private:
        Rotation rotation_;
    };

    class InvertColorCommand : public Command
    {
    public:
        explicit InvertColorCommand(std::string name) : Command(name){};

        virtual void Invoke(AbstractImage &im) override { im.InvertColors(); };
    };

    class GrayscaleCommand : public Command
    {
    public:
        explicit GrayscaleCommand(std::string name) : Command(name){};

        virtual void Invoke(AbstractImage &im) override { im.ConvertToGrayscale(); };
    };

    class CropCommand : public Command
    {
    public:
        explicit CropCommand(std::string name) : Command(name){};

        virtual void Invoke(AbstractImage &im) override { im.Crop(); };

    private:
        std::shared_ptr<BasePoint> point1_;
        std::shared_ptr<BasePoint> point2_;
    };

    class UndoCommand : public Command
    {
    public:
        explicit UndoCommand(std::string name) : Command(name){};

        virtual void Invoke(AbstractImage &im) override { im.Undo(); };
    };

    class RedoCommand : public Command
    {
    public:
        explicit RedoCommand(std::string name) : Command(name){};

        virtual void Invoke(AbstractImage &im) override { im.Redo(); };
    };
}

#endif // PAINT_INC_COMMAND_H_