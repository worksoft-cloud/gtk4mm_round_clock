#include <gtkmm.h>
#include <cmath>
#include <ctime>

class MyWidget : public Gtk::Widget {
public:
    MyWidget() {
        set_overflow(Gtk::Overflow::HIDDEN);
    }

protected:
    void measure_vfunc(Gtk::Orientation orientation,
                       int for_size,
                       int& minimum,
                       int& natural,
                       int& minimum_baseline,
                       int& natural_baseline) const override
    {
        minimum = natural = 100; // simple fixed size
    }

    void size_allocate_vfunc(int width, int height, int baseline) override
    {
        Gtk::Widget::size_allocate_vfunc(width, height, baseline);
    }

    void snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot) override
    {
        Gdk::RGBA color("cornflowerblue");
//        snapshot->append_color(color, {0, 0, get_width(), get_height()});
    }
};

class ClockWindow : public Gtk::Window {
public:
    ClockWindow() {
        set_title("GTKmm Clock");
        set_default_size(350, 350);

        area.set_draw_func(sigc::mem_fun(*this, &ClockWindow::draw_clock));

        Glib::signal_timeout().connect(
            sigc::mem_fun(*this, &ClockWindow::on_timeout), 100);

        set_child(area);
    }

private:
    Gtk::DrawingArea area;
    MyWidget mywidget;

    bool on_timeout() {
        area.queue_draw();
        return true;
    }

    void draw_clock(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height) {
        const double cx = width / 2.0;
        const double cy = height / 2.0;
        const double radius = std::min(width, height) * 0.45;

        cr->set_source_rgb(1, 1, 1);
        cr->paint();

        cr->set_line_width(4.0);
        cr->set_source_rgb(0, 0, 0);
        cr->arc(cx, cy, radius, 0, 2 * M_PI);
        cr->stroke();

        draw_ticks(cr, cx, cy, radius);
        draw_numbers(cr, cx, cy, radius);

        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);

        double sec = now->tm_sec;
        double min = now->tm_min + sec / 60.0;
        double hour = (now->tm_hour % 12) + min / 60.0;

        auto angle = [](double v) { return (v * M_PI / 30.0) - M_PI/2.0; };

        cr->set_line_width(6.0);
        cr->move_to(cx, cy);
        cr->line_to(cx + std::cos(angle(hour * 5)) * radius * 0.5,
                    cy + std::sin(angle(hour * 5)) * radius * 0.5);
        cr->stroke();

        cr->set_line_width(4.0);
        cr->move_to(cx, cy);
        cr->line_to(cx + std::cos(angle(min)) * radius * 0.75,
                    cy + std::sin(angle(min)) * radius * 0.75);
        cr->stroke();

        cr->set_source_rgb(1, 0, 0);
        cr->set_line_width(2.0);
        cr->move_to(cx, cy);
        cr->line_to(cx + std::cos(angle(sec)) * radius * 0.85,
                    cy + std::sin(angle(sec)) * radius * 0.85);
        cr->stroke();
    }

    void draw_ticks(const Cairo::RefPtr<Cairo::Context>& cr,
                    double cx, double cy, double radius) {
        for (int i = 0; i < 60; ++i) {
            double angle = i * M_PI / 30.0;
            double inner = (i % 5 == 0) ? radius * 0.80 : radius * 0.88;
            double outer = radius;

            cr->set_line_width((i % 5 == 0) ? 3.0 : 1.5);
            cr->set_source_rgb(0, 0, 0);

            cr->move_to(cx + std::cos(angle) * inner,
                        cy + std::sin(angle) * inner);
            cr->line_to(cx + std::cos(angle) * outer,
                        cy + std::sin(angle) * outer);
            cr->stroke();
        }
    }

    void draw_numbers(const Cairo::RefPtr<Cairo::Context>& cr,
                      double cx, double cy, double radius) {
        cr->set_source_rgb(0, 0, 0);
        cr->set_font_size(radius * 0.12);

        for (int i = 1; i <= 12; ++i) {
            double angle = (i * M_PI / 6.0) - M_PI/2.0;
            double tx = cx + std::cos(angle) * radius * 0.65;
            double ty = cy + std::sin(angle) * radius * 0.65;

            Cairo::TextExtents ext;
            cr->get_text_extents(std::to_string(i), ext);

            cr->move_to(tx - ext.width / 2.0, ty + ext.height / 2.0);
            cr->show_text(std::to_string(i));
        }
    }
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("example.clock");
    return app->make_window_and_run<ClockWindow>(argc, argv);
}
