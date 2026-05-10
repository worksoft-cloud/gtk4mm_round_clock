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
        //snapshot->append_color(color, {0, 0, get_width(), get_height()});
    }
};
