#include "graphics.h"
#include "csv.h"
#include "paths.h"

using namespace std;
using namespace mssm;
using namespace fmt;

class Range {
public:
    double minVal;
    double maxVal;
public:
    constexpr Range() { *this = none; }
    constexpr Range(double minVal, double maxVal) : minVal{ minVal }, maxVal{ maxVal } {}
    void setStartSize(double start, double size) { minVal = start; maxVal = start+size; }
    static Range all = { std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max() };
    static Range none = { std::numeric_limits<double>::max(), std::numeric_limits<double>::min() };
    constexpr double size() const { return maxVal - minVal; }
    void extend(double value);
    constexpr double fromT(double t) const { return minVal + t * size(); }
    constexpr double toT(double value) const { return (value - minVal) / size(); }
    constexpr double mapToOtherRange(const Range& other, double value) const { return other.fromT(toT(value)); }
    constexpr double mapFromOtherRange(const Range& other, double value) const { return fromT(range.toT(value)); }
};

void Range::extend(double value)
{
    if (value > maxVal) {
        maxVal = value;
    }
    if (value < minVal) {
        minVal = value;
    }
}

class Range2d {
public:
    Range xRange;
    Range yRange;
public:
    Range() {}
    void extendY(double value) { yRange.extend(value); }
    void extendX(double value) { xRange.extend(value); }
    void extend(Vec2d value) { xRange.extend(value.x); yRange.extend(value.y); }
    Vec2d start
    constexpr double width() const { return xRange.size(); }
    constexpr double height() const { return yRange.size(); }
    constexpr Vec2d fromT(Vec2d t) const { return { xRange.fromT(t.x), yRange.fromT(t.y) }; }
    constexpr Vec2d toT(Vec2d value) const { return { xRange.toT(t.x), yRange.toT(t.y) }; }
    constexpr Vec2d mapToOtherRange(const Range2d& other, Vec2d value) const {
        return { xRange.mapToOtherRange(other.xRange, value.x),
                 yRange.mapToOtherRange(other.yRange, value.y)};
    }
    constexpr Vec2d mapFromOtherRange(const Range2d& other, Vec2d value) const {
        return { xRange.mapFromOtherRange(other.xRange, value.x),
                yRange.mapFromOtherRange(other.yRange, value.y)};
    }
};

class ViewPort {
public:
    Range2d viewRange;
    Range2d dataRange;
public:
    Vec2d toScreen(Vec2d p) const;
    Vec2d toModel(Vec2d screenPos) const;
    void draw(Graphics& g) const;
    double width() const { return maxRange.x-minRange.x; }
    double height() const { return maxRange.y-minRange.y; }
    void drawAxes(Graphics& g) const;
    void drawPoints(Graphics& g, std::vector<Vec2d> points, Color c) const;
    void fitPoints(const std::vector<Vec2d>& points);
};

Vec2d ViewPort::toScreen(Vec2d p) const
{
    p -= minRange;
    p.x /= width();
    p.y /= height();
    p.x *= windowSize.x;
    p.y *= windowSize.y;
    p += windowPos;
    return p;
}

Vec2d ViewPort::toModel(Vec2d screenPos) const
{
    screenPos -= windowPos;
    screenPos.x /= windowSize.x;
    screenPos.y /= windowSize.y;
    screenPos.x *= width();
    screenPos.y *= height();
    screenPos += minRange;
    return screenPos;
}

void ViewPort::draw(Graphics &g) const
{
    g.rect(windowPos, windowSize.x, windowSize.y, WHITE);
}

void ViewPort::drawPoints(Graphics &g, std::vector<Vec2d> points, Color c) const
{
    for (auto& p : points) {
        p = toScreen(p);
    }

    g.points(points, c);
}

void findRange(const std::vector<Vec2d> &points, Vec2d& minExtent, Vec2d& maxExtent)
{
    minExtent = { std::numeric_limits<double>::max(), std::numeric_limits<double>::max() };
    maxExtent = { std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest() };

    for (auto& p : points) {
        if (p.x > maxExtent.x) { maxExtent.x = p.x; }
        if (p.y > maxExtent.y) { maxExtent.y = p.y; }
        if (p.x < minExtent.x) { minExtent.x = p.x; }
        if (p.y < minExtent.y) { minExtent.y = p.y; }
    }
}

void ViewPort::fitPoints(const std::vector<Vec2d> &points)
{
    findRange(points, minRange, maxRange);
}

int main()
{
    Graphics g("Testing", 1024, 768);

    ViewPort vp;

    vector<Vec2d> data;

    io::CSVReader<2> in(Paths::findAsset("data/linear_regression.csv"));
    in.read_header(io::ignore_extra_column, "SAT", "GPA");
    double sat;
    double gpa;
    while(in.read_row(sat, gpa)){
        data.push_back({sat,gpa});
    }

    vp.fitPoints(data);

    while (g.draw()) {
        if (!g.isDrawable()) {
            continue;
        }

        vp.draw(g);

        Vec2d mp = vp.toModel(g.mousePos());
        g.println("{}", mp);

        vp.drawPoints(g, data, GREEN);
    }

    return 0;
}


