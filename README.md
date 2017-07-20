# Paradigm Brake Module controller

This controller implements the Realtime brake control logic for Paradigm air
brakes. The module receives instructions on desired operating mode and desired
force setpoint from a master node over UDP. Should a packet

# Dev Notes

This project uses the Arduino Bootloader and is written in Arduino C++ with
heavy preference to the standard C dialect to keep things simple.  In the
future templates of the `node_t` struct should be implemented to abstract a
common node and overlay additional functionality on top of it.

# License

See the [LICENSE](LICENSE) for full licensing details.

In summary, (you still need to read the whole thing), these boards are for Paradigm,
and Paradigm only. It is shared with the world for the benefit of observers and
potential Paradigm team members. If you wish to utilize these designs in any
way, you must contact us first and receive written permission to utilize the
designs for the purpose you require.

We don't want to be restrictive, and we will almost certainly grant you permission,
but we want to know what you are using our designs for, and we want to make sure
you are using them correctly and safely.
