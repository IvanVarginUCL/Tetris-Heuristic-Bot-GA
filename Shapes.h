#pragma once

#include <vector>

struct Coords {
    int y; int x;

    bool operator==(const Coords& a) const;
};


const int SHAPESIZE = 4, NUMSHAPES = 7;


const std::vector<Coords>  HLINE4{ {0, 0}, {0, 1}, {0, 2}, {0, 3} };
const std::vector<Coords>  VLINE4{ {0, 0}, {1, 0}, {2, 0}, {3, 0} };

const std::vector<Coords>  L1{ {0, 0}, {1, 0}, {2, 0}, {2, 1} };
const std::vector<Coords>  L2{ {0, 0}, {0, 1}, {0, 2}, {1, 0} };
const std::vector<Coords>  L3{ {0, 0}, {0, 1}, {1, 1}, {2, 1} };
const std::vector<Coords>  L4{ {0, 2}, {1, 0}, {1, 1}, {1, 2} };

const std::vector<Coords>  LM1{ {0, 1}, {1, 1}, {2, 0}, {2, 1} };
const std::vector<Coords>  LM2{ {0, 0}, {1, 0}, {1, 1}, {1, 2} };
const std::vector<Coords>  LM3{ {0, 0}, {0, 1}, {1, 0}, {2, 0} };
const std::vector<Coords>  LM4{ {0, 0}, {0, 1}, {0, 2}, {1, 2} };

const std::vector<Coords>  BOX{ {0, 0}, {0, 1}, {1, 0}, {1, 1} };

const std::vector<Coords>  SDONG1{ {0, 1}, {1, 0}, {1, 1}, {1, 2} };
const std::vector<Coords>  SDONG2{ {0, 0}, {1, 0}, {1, 1}, {2, 0} };
const std::vector<Coords>  SDONG3{ {0, 0}, {0, 1}, {0, 2}, {1, 1} };
const std::vector<Coords>  SDONG4{ {0, 1}, {1, 0}, {1, 1}, {2, 1} };

const std::vector<Coords>  STAIR1{ {0, 1}, {1, 0}, {1, 1}, {2, 1} };
const std::vector<Coords>  STAIR2{ {0, 1}, {0, 2}, {1, 0}, {1, 1} };

const std::vector<Coords>  STAIRM1{ {0, 1}, {1, 0}, {1, 1}, {2, 0} };
const std::vector<Coords>  STAIRM2{ {0, 0}, {0, 1}, {1, 1}, {1, 2} };


const std::vector<std::vector<Coords>> LINE { HLINE4, VLINE4, HLINE4, VLINE4 };
const std::vector<std::vector<Coords>> L { L1, L2, L3, L4 };
const std::vector<std::vector<Coords>> L_M { LM1, LM2, LM3, LM4 };
const std::vector<std::vector<Coords>> T { SDONG3, SDONG4, SDONG1, SDONG2 };
const std::vector<std::vector<Coords>> SQUARE { BOX, BOX, BOX, BOX };
const std::vector<std::vector<Coords>> STAIR { STAIR2, STAIR1, STAIR2, STAIR1 };
const std::vector<std::vector<Coords>> STAIR_M { STAIRM2, STAIRM1, STAIRM2, STAIRM1 };


const std::vector<std::vector<Coords>> SHAPES[NUMSHAPES] = { LINE, L, L_M, T, SQUARE, STAIR, STAIR_M };