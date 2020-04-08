set_global_assignment -entity "VGA_CLOCK_GEN" -library "lib_VGA_CLOCK_GEN" -name IP_TOOL_NAME "altera_pll"
set_global_assignment -entity "VGA_CLOCK_GEN" -library "lib_VGA_CLOCK_GEN" -name IP_TOOL_VERSION "15.0"
set_global_assignment -entity "VGA_CLOCK_GEN" -library "lib_VGA_CLOCK_GEN" -name IP_TOOL_ENV "mwpim"
set_global_assignment -library "lib_VGA_CLOCK_GEN" -name SPD_FILE [file join $::quartus(sip_path) "VGA_CLOCK_GEN.spd"]

set_global_assignment -library "lib_VGA_CLOCK_GEN" -name MISC_FILE [file join $::quartus(sip_path) "VGA_CLOCK_GEN_sim/VGA_CLOCK_GEN.vho"]
