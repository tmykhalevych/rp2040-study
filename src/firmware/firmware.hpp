#pragma once

#include <prohibit_copy_move.hpp>

namespace fw
{

class Firmware : public cmn::ProhibitCopyMove
{
public:
    Firmware();

    void start() const;

private:
    void root_thread();
};

}  // namespace fw
