#include "Colours.h"

namespace my_colours{
    std::map<std::string, Vec3f> colours = {
        {"yellow", Vec3f(0.98, 1.0, 0.9)},
        {"purple", Vec3f(0.98, 0.7, 0.85)},
        {"green", Vec3f(0.8, 0.95, 0.6)},
        {"white", Vec3f(1.0, 1.0, 1.0)},
        {"black", Vec3f(0.0, 0.0, 0.0)},
        {"grey1", Vec3f(0.5, 0.5, 0.5)},
        {"grey2", Vec3f(0.75, 0.75, 0.75)},
        {"blue", Vec3f(0.1, 0.4, 0.8)},
        {"turquoise", Vec3f(0.6, 0.95, 0.8)},
        {"watercolour", Vec3f(0.6, 0.9, 1.0)},
        {"red", Vec3f(0.9568627450980392, 0.2784313725490196, 0.2784313725490196)},
        {"gemcolor", Vec3f(0.0, 0.9, 1.0)},
        {"orange", Vec3f(1.0, 0.6039, 0.0)},
        {"teal", Vec3f(0.1529, 0.6549, 0.8471)},
        {"pink", Vec3f(1.0, 0.50196, 0.75294)}
    };
}