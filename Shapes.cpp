#include "Shapes.h"

bool Coords::operator==(const Coords& a) const
{
    return (x == a.x && y == a.y);
}