set version "2015.2"
set project_name "zvik_camera_${version}"
set project_dir "project"
set ip_dir "srcs/ip"
set hdl_dir "srcs/hdl"
set ui_dir "srcs/ui"
set constrs_dir "constrs"
set scripts_dir "scripts"
set bd_name "system_top"
set part "xc7z010clg400-1"
set board "digilentinc.com:zybo:part0:1.0"

# set up project
create_project $project_name $project_dir -part $part -force
set_property board $board [current_project]

# set up IP repo
set_property ip_repo_paths $ip_dir [current_fileset]
update_ip_catalog -rebuild

create_bd_design "design_1"


startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system7_1
create_bd_cell -type ip -vlnv xilinx.com:user:ov7725_top:1.0 ov7725_top_1
create_bd_cell -type ip -vlnv xilinx.com:ip:v_tc:6.1 v_tc_1
create_bd_cell -type ip -vlnv xilinx.com:ip:v_vid_in_axi4s:3.0 v_vid_in_axi4s_1
create_bd_cell -type ip -vlnv xilinx.com:ip:v_axi4s_vid_out:3.0 v_axi4s_vid_out_1
create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 vcc
create_bd_cell -type ip -vlnv xilinx.com:ip:xlconstant:1.1 gnd
create_bd_cell -type ip -vlnv xilinx.com:ip:clk_wiz:5.1 clk_wiz_1
create_bd_cell -type ip -vlnv xilinx.com:ip:axis_broadcaster:1.1 axis_broadcaster_1

create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vdma:6.2 axi_vdma_1
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_vdma:6.2 axi_vdma_2

create_bd_cell -type ip -vlnv xilinx.com:ip:xlconcat:2.1 xlconcat_1
create_bd_cell -type ip -vlnv xilinx.com:hls:sobel:1.0 sobel_1

create_bd_cell -type ip -vlnv xilinx.com:ip:microblaze:9.5 microblaze_0
create_bd_cell -type ip -vlnv xilinx.com:user:motor_control:1.0 motor_control_0
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_uartlite:2.0 axi_uartlite_0
create_bd_cell -type ip -vlnv xilinx.com:user:ultrasonic_control:1.0 ultrasonic_control_0
create_bd_cell -type ip -vlnv xilinx.com:user:speed_control:1.0 speed_control_0
create_bd_cell -type ip -vlnv xilinx.com:user:instruction_PL2PS_PS2PL:1.0 instruction_PL2PS_PS2PL_0
create_bd_cell -type ip -vlnv xilinx.com:user:instruction_PL2PS_PS2PL:1.0 instruction_PL2PS_PS2PL_1
endgroup


set_property -dict [list CONFIG.NUM_PORTS {3}] [get_bd_cells xlconcat_1]
set_property -dict [list CONFIG.c_m_axi_s2mm_data_width {32} CONFIG.c_include_mm2s {0} CONFIG.c_include_s2mm_dre {1} CONFIG.c_s2mm_linebuffer_depth {512}] [get_bd_cells /axi_vdma_1]
set_property -dict [list CONFIG.c_m_axi_s2mm_data_width {32} CONFIG.c_include_mm2s {0} CONFIG.c_include_s2mm_dre {1} CONFIG.c_s2mm_linebuffer_depth {512}] [get_bd_cells /axi_vdma_2]


set_property -dict [list CONFIG.M_TDATA_NUM_BYTES {3} CONFIG.S_TDATA_NUM_BYTES {3} CONFIG.M_TUSER_WIDTH {1} CONFIG.S_TUSER_WIDTH {1} CONFIG.HAS_TLAST {1}] [get_bd_cells /axis_broadcaster_1]


# system 7_1
set_property -dict [list CONFIG.PCW_USE_S_AXI_GP0 {1} CONFIG.PCW_USE_S_AXI_HP0 {1} CONFIG.PCW_USE_FABRIC_INTERRUPT {1} CONFIG.PCW_IRQ_F2P_INTR {1} CONFIG.PCW_P2F_UART1_INTR {1} CONFIG.PCW_EN_CLK2_PORT {1}] [get_bd_cells processing_system7_1]
apply_bd_automation -rule xilinx.com:bd_rule:processing_system7 -config {make_external "FIXED_IO, DDR" apply_board_preset "1" Master "Disable" Slave "Disable" }  [get_bd_cells processing_system7_1]
set_property -dict [list CONFIG.PCW_FPGA0_PERIPHERAL_FREQMHZ {25} CONFIG.PCW_USE_S_AXI_HP0 {1}] [get_bd_cells processing_system7_1]
set_property -dict [list CONFIG.PCW_USE_S_AXI_HP1 {1}] [get_bd_cells processing_system7_1]
set_property -dict [list CONFIG.PCW_FPGA1_PERIPHERAL_FREQMHZ {25.5} CONFIG.PCW_EN_CLK1_PORT {1}] [get_bd_cells processing_system7_1]

set_property -dict [list CONFIG.CONST_VAL {0}] [get_bd_cells gnd]
set_property -dict [list CONFIG.CONST_VAL {1}] [get_bd_cells vcc]
set_property -dict [list CONFIG.C_M_AXIS_VIDEO_FORMAT {2}] [get_bd_cells v_vid_in_axi4s_1]

# clk_wizard
set_property -dict [list CONFIG.PRIM_IN_FREQ.VALUE_SRC USER] [get_bd_cells clk_wiz_1]
set_property -dict [list CONFIG.PRIM_IN_FREQ {25.000} CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {25.000} CONFIG.USE_LOCKED {false} CONFIG.USE_RESET {false} CONFIG.CLKIN1_JITTER_PS {393.7} CONFIG.MMCM_DIVCLK_DIVIDE {1} CONFIG.MMCM_CLKFBOUT_MULT_F {36.000} CONFIG.MMCM_CLKIN1_PERIOD {39.37} CONFIG.MMCM_CLKOUT0_DIVIDE_F {36.000} CONFIG.CLKOUT1_JITTER {396.400} CONFIG.CLKOUT1_PHASE_ERROR {245.093}] [get_bd_cells clk_wiz_1]


connect_bd_net [get_bd_pins ov7725_top_1/clk25out] [get_bd_pins clk_wiz_1/clk_in1]

# pins of camera driver
create_bd_port -dir O OV7725_SIOC
connect_bd_net [get_bd_pins /ov7725_top_1/OV7725_SIOC] [get_bd_ports OV7725_SIOC]
create_bd_port -dir I clkInit
connect_bd_net [get_bd_pins /ov7725_top_1/clkInit] [get_bd_ports clkInit]
create_bd_port -dir I OV7725_VSYNC
connect_bd_net [get_bd_pins /ov7725_top_1/OV7725_VSYNC] [get_bd_ports OV7725_VSYNC]
create_bd_port -dir IO OV7725_SIOD
connect_bd_net [get_bd_pins /ov7725_top_1/OV7725_SIOD] [get_bd_ports OV7725_SIOD]
create_bd_port -dir O -from 3 -to 0 LED
connect_bd_net [get_bd_pins /ov7725_top_1/LED] [get_bd_ports LED]
create_bd_port -dir I -from 7 -to 0 OV7725_D
connect_bd_net [get_bd_pins /ov7725_top_1/OV7725_D] [get_bd_ports OV7725_D]
create_bd_port -dir I OV7725_PCLK
connect_bd_net [get_bd_pins /ov7725_top_1/OV7725_PCLK] [get_bd_ports OV7725_PCLK]
create_bd_port -dir I OV7725_HREF
connect_bd_net [get_bd_pins /ov7725_top_1/OV7725_HREF] [get_bd_ports OV7725_HREF]
create_bd_port -dir O OV7725_XCLK
connect_bd_net [get_bd_pins /ov7725_top_1/OV7725_XCLK] [get_bd_ports OV7725_XCLK]
create_bd_port -dir I btn
connect_bd_net [get_bd_pins /ov7725_top_1/btn] [get_bd_ports btn]


connect_bd_net [get_bd_pins ov7725_top_1/rbg] [get_bd_pins v_vid_in_axi4s_1/vid_data]
connect_bd_net [get_bd_pins ov7725_top_1/valid] [get_bd_pins v_vid_in_axi4s_1/vid_active_video]
connect_bd_net [get_bd_pins ov7725_top_1/vga_hsync] [get_bd_pins v_vid_in_axi4s_1/vid_hsync]
connect_bd_net [get_bd_pins ov7725_top_1/vga_vsync] [get_bd_pins v_vid_in_axi4s_1/vid_vsync]
connect_bd_net -net [get_bd_nets gnd_dout] [get_bd_pins v_vid_in_axi4s_1/rst] [get_bd_pins gnd/dout]
connect_bd_net -net [get_bd_nets vcc_dout] [get_bd_pins v_vid_in_axi4s_1/aclken] [get_bd_pins vcc/dout]
connect_bd_net -net [get_bd_nets vcc_dout] [get_bd_pins v_vid_in_axi4s_1/aresetn] [get_bd_pins vcc/dout]
connect_bd_net -net [get_bd_nets vcc_dout] [get_bd_pins v_vid_in_axi4s_1/axis_enable] [get_bd_pins vcc/dout]
connect_bd_net -net [get_bd_nets vcc_dout] [get_bd_pins v_vid_in_axi4s_1/vid_io_in_ce] [get_bd_pins vcc/dout]




set_property -dict [list CONFIG.HAS_AXI4_LITE {false} CONFIG.VIDEO_MODE {640x480p} CONFIG.enable_detection {false} CONFIG.GEN_F0_VSYNC_VSTART {489} CONFIG.GEN_F1_VSYNC_VSTART {489} CONFIG.GEN_HACTIVE_SIZE {640} CONFIG.GEN_HSYNC_END {752} CONFIG.GEN_HFRAME_SIZE {800} CONFIG.GEN_F0_VSYNC_HSTART {640} CONFIG.GEN_F1_VSYNC_HSTART {640} CONFIG.GEN_F0_VSYNC_HEND {640} CONFIG.GEN_F1_VSYNC_HEND {640} CONFIG.GEN_F0_VFRAME_SIZE {525} CONFIG.GEN_F1_VFRAME_SIZE {525} CONFIG.GEN_F0_VSYNC_VEND {491} CONFIG.GEN_F1_VSYNC_VEND {491} CONFIG.GEN_F0_VBLANK_HEND {640} CONFIG.GEN_F1_VBLANK_HEND {640} CONFIG.GEN_HSYNC_START {656} CONFIG.GEN_VACTIVE_SIZE {480} CONFIG.GEN_F0_VBLANK_HSTART {640} CONFIG.GEN_F1_VBLANK_HSTART {640}] [get_bd_cells v_tc_1]


connect_bd_net -net [get_bd_nets vcc_dout] [get_bd_pins v_tc_1/clken] [get_bd_pins vcc/dout]
connect_bd_net -net [get_bd_nets vcc_dout] [get_bd_pins v_tc_1/resetn] [get_bd_pins vcc/dout]

connect_bd_intf_net [get_bd_intf_pins v_tc_1/vtiming_out] [get_bd_intf_pins v_axi4s_vid_out_1/vtiming_in]
connect_bd_net [get_bd_pins v_axi4s_vid_out_1/vtg_ce] [get_bd_pins v_tc_1/gen_clken]
connect_bd_net -net [get_bd_nets gnd_dout] [get_bd_pins v_axi4s_vid_out_1/rst] [get_bd_pins gnd/dout]
connect_bd_net -net [get_bd_nets vcc_dout] [get_bd_pins v_axi4s_vid_out_1/vid_io_out_ce] [get_bd_pins vcc/dout]
connect_bd_net -net [get_bd_nets vcc_dout] [get_bd_pins v_axi4s_vid_out_1/aresetn] [get_bd_pins vcc/dout]
connect_bd_net -net [get_bd_nets vcc_dout] [get_bd_pins v_axi4s_vid_out_1/aclken] [get_bd_pins vcc/dout]


connect_bd_net [get_bd_pins clk_wiz_1/clk_out1] [get_bd_pins v_vid_in_axi4s_1/aclk]
connect_bd_net -net [get_bd_nets clk_wiz_1_clk_out1] [get_bd_pins v_vid_in_axi4s_1/vid_io_in_clk] [get_bd_pins clk_wiz_1/clk_out1]

#video connection
connect_bd_intf_net [get_bd_intf_pins v_vid_in_axi4s_1/video_out]  [get_bd_intf_pins axis_broadcaster_1/S_AXIS]

connect_bd_intf_net [get_bd_intf_pins sobel_1/video_in] [get_bd_intf_pins axis_broadcaster_1/M01_AXIS]
connect_bd_intf_net [get_bd_intf_pins sobel_1/video_out] [get_bd_intf_pins axi_vdma_2/S_AXIS_S2MM]
connect_bd_intf_net [get_bd_intf_pins axis_broadcaster_1/M00_AXIS] [get_bd_intf_pins axi_vdma_1/S_AXIS_S2MM]
connect_bd_net -net [get_bd_nets clk_wiz_1_clk_out1] [get_bd_pins sobel_1/ap_clk] [get_bd_pins clk_wiz_1/clk_out1]

#concat
connect_bd_net [get_bd_pins processing_system7_1/IRQ_F2P] [get_bd_pins xlconcat_1/dout]
connect_bd_net [get_bd_pins axi_vdma_1/s2mm_introut] [get_bd_pins xlconcat_1/In0]
connect_bd_net [get_bd_pins xlconcat_1/In1] [get_bd_pins axi_vdma_2/s2mm_introut]


#set_property -dict [list CONFIG.PCW_USE_S_AXI_HP0 {0}] [get_bd_cells processing_system7_1]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/processing_system7_1/M_AXI_GP0" Clk "/processing_system7_1/FCLK_CLK0 (25 MHz)" }  [get_bd_intf_pins axi_vdma_1/S_AXI_LITE]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/processing_system7_1/M_AXI_GP0" Clk "/processing_system7_1/FCLK_CLK0 (25 MHz)" }  [get_bd_intf_pins axi_vdma_2/S_AXI_LITE]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/processing_system7_1/M_AXI_GP0" Clk "Auto" }  [get_bd_intf_pins sobel_1/s_axi_CONTROL_BUS]

connect_bd_net -net [get_bd_nets clk_wiz_1_clk_out1] [get_bd_pins processing_system7_1/S_AXI_HP0_ACLK] [get_bd_pins clk_wiz_1/clk_out1]
connect_bd_net -net [get_bd_nets clk_wiz_1_clk_out1] [get_bd_pins axi_vdma_1/s_axis_s2mm_aclk] [get_bd_pins clk_wiz_1/clk_out1]
connect_bd_net -net [get_bd_nets clk_wiz_1_clk_out1] [get_bd_pins axi_vdma_1/m_axi_s2mm_aclk] [get_bd_pins clk_wiz_1/clk_out1]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/axi_vdma_1/M_AXI_S2MM" Clk "Auto" }  [get_bd_intf_pins processing_system7_1/S_AXI_HP0]

connect_bd_net -net [get_bd_nets clk_wiz_1_clk_out1] [get_bd_pins axis_broadcaster_1/aclk] [get_bd_pins clk_wiz_1/clk_out1]
connect_bd_net -net [get_bd_nets vcc_dout] [get_bd_pins axis_broadcaster_1/aresetn] [get_bd_pins vcc/dout]

connect_bd_net -net [get_bd_nets clk_wiz_1_clk_out1] [get_bd_pins processing_system7_1/S_AXI_HP1_ACLK] [get_bd_pins clk_wiz_1/clk_out1]
connect_bd_net -net [get_bd_nets clk_wiz_1_clk_out1] [get_bd_pins axi_vdma_2/s_axis_s2mm_aclk] [get_bd_pins clk_wiz_1/clk_out1]
connect_bd_net -net [get_bd_nets clk_wiz_1_clk_out1] [get_bd_pins axi_vdma_2/m_axi_s2mm_aclk] [get_bd_pins clk_wiz_1/clk_out1]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/axi_vdma_2/M_AXI_S2MM" Clk "Auto" }  [get_bd_intf_pins processing_system7_1/S_AXI_HP1]

connect_bd_net [get_bd_pins processing_system7_1/FCLK_CLK1] [get_bd_pins v_axi4s_vid_out_1/aclk]
connect_bd_net -net [get_bd_nets processing_system7_1_FCLK_CLK1] [get_bd_pins v_axi4s_vid_out_1/vid_io_out_clk] [get_bd_pins processing_system7_1/FCLK_CLK1]
connect_bd_net -net [get_bd_nets processing_system7_1_FCLK_CLK1] [get_bd_pins v_tc_1/clk] [get_bd_pins processing_system7_1/FCLK_CLK1]
connect_bd_net -net [get_bd_nets vcc_dout] [get_bd_pins rst_clk_wiz_1_25M/ext_reset_in] [get_bd_pins vcc/dout]

create_bd_port -dir O -from 23 -to 0 vga
connect_bd_net [get_bd_pins /v_axi4s_vid_out_1/vid_data] [get_bd_ports vga]
create_bd_port -dir O vga_hsync
connect_bd_net [get_bd_pins /v_axi4s_vid_out_1/vid_hsync] [get_bd_ports vga_hsync]
create_bd_port -dir O vga_vsync
connect_bd_net [get_bd_pins /v_axi4s_vid_out_1/vid_vsync] [get_bd_ports vga_vsync]

# Configuration of IP's

## Microblaze
apply_bd_automation -rule xilinx.com:bd_rule:microblaze -config {local_mem "128KB" ecc "None" cache "8KB" debug_module "Debug Only" axi_periph "Enabled" axi_intc "1" clk "/processing_system7_1/FCLK_CLK2 (50 MHz)" }  [get_bd_cells microblaze_0]
set_property -dict [list CONFIG.G_USE_EXCEPTIONS {1}] [get_bd_cells microblaze_0]

# Axi Connexions

## Microblaze Peripherals
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "/processing_system7_1/FCLK_CLK2 (50 MHz)" }  [get_bd_intf_pins motor_control_0/S00_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "/processing_system7_1/FCLK_CLK2 (50 MHz)" }  [get_bd_intf_pins axi_uartlite_0/S_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "/processing_system7_1/FCLK_CLK2 (50 MHz)" }  [get_bd_intf_pins ultrasonic_control_0/S00_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "/processing_system7_1/FCLK_CLK2 (50 MHz)" }  [get_bd_intf_pins ultrasonic_control_0/S_AXI_INTR]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Periph)" Clk "/processing_system7_1/FCLK_CLK2 (50 MHz)" }  [get_bd_intf_pins speed_control_0/S00_AXI]

## Zynq Peripherals
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/processing_system7_1/M_AXI_GP0" Clk "/processing_system7_1/FCLK_CLK2 (50 MHz)" }  [get_bd_intf_pins instruction_PL2PS_PS2PL_0/S00_AXI]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/processing_system7_1/M_AXI_GP0" Clk "/processing_system7_1/FCLK_CLK2 (50 MHz)" }  [get_bd_intf_pins instruction_PL2PS_PS2PL_1/S00_AXI]


## Shared Peripherals
set_property -dict [list CONFIG.NUM_SI {2} CONFIG.NUM_MI {5}] [get_bd_cells processing_system7_1_axi_periph]
set_property -dict [list CONFIG.NUM_MI {8}] [get_bd_cells microblaze_0_axi_periph]

connect_bd_intf_net -boundary_type upper [get_bd_intf_pins microblaze_0_axi_periph/M06_AXI] [get_bd_intf_pins processing_system7_1/S_AXI_GP0]
connect_bd_net -net [get_bd_nets microblaze_0_Clk] [get_bd_pins microblaze_0_axi_periph/M06_ACLK] [get_bd_pins processing_system7_1/FCLK_CLK2]
connect_bd_net -net [get_bd_nets rst_processing_system7_1_50M_peripheral_aresetn] [get_bd_pins microblaze_0_axi_periph/M06_ARESETN] [get_bd_pins rst_processing_system7_1_50M/peripheral_aresetn]
connect_bd_net -net [get_bd_nets microblaze_0_Clk] [get_bd_pins processing_system7_1/S_AXI_GP0_ACLK] [get_bd_pins processing_system7_1/FCLK_CLK2]

connect_bd_intf_net -boundary_type upper [get_bd_intf_pins microblaze_0_axi_periph/M07_AXI] [get_bd_intf_pins processing_system7_1_axi_periph/S01_AXI]
connect_bd_net -net [get_bd_nets microblaze_0_Clk] [get_bd_pins processing_system7_1_axi_periph/S01_ACLK] [get_bd_pins processing_system7_1/FCLK_CLK2]
connect_bd_net -net [get_bd_nets rst_processing_system7_1_50M_peripheral_aresetn] [get_bd_pins processing_system7_1_axi_periph/S01_ARESETN] [get_bd_pins rst_processing_system7_1_50M/peripheral_aresetn]
connect_bd_net -net [get_bd_nets microblaze_0_Clk] [get_bd_pins microblaze_0_axi_periph/M07_ACLK] [get_bd_pins processing_system7_1/FCLK_CLK2]
connect_bd_net -net [get_bd_nets rst_processing_system7_1_50M_peripheral_aresetn] [get_bd_pins microblaze_0_axi_periph/M07_ARESETN] [get_bd_pins rst_processing_system7_1_50M/peripheral_aresetn]

apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config {Master "/microblaze_0 (Cached)" Clk "/processing_system7_1/FCLK_CLK2 (50 MHz)" }  [get_bd_intf_pins processing_system7_1/S_AXI_HP0]

# Pin Connexions

## Interruption

### Microblaze
set_property -dict [list CONFIG.NUM_PORTS {4}] [get_bd_cells microblaze_0_xlconcat]
connect_bd_net [get_bd_pins ultrasonic_control_0/us_irq] [get_bd_pins microblaze_0_xlconcat/In0]
connect_bd_net [get_bd_pins instruction_PL2PS_PS2PL_0/irq] [get_bd_pins microblaze_0_xlconcat/In1]
connect_bd_net [get_bd_pins axi_uartlite_0/interrupt] [get_bd_pins microblaze_0_xlconcat/In2]
connect_bd_net [get_bd_pins processing_system7_1/IRQ_P2F_UART1] [get_bd_pins microblaze_0_xlconcat/In3]

### Zynq
connect_bd_net [get_bd_pins instruction_PL2PS_PS2PL_1/irq] [get_bd_pins xlconcat_1/In2]

## External Ports
create_bd_port -dir O -from 3 -to 0 MT_PWM_LEFT
connect_bd_net [get_bd_pins /motor_control_0/pwm_out_left] [get_bd_ports MT_PWM_LEFT]

create_bd_port -dir O -from 3 -to 0 MT_PWM_RIGHT
connect_bd_net [get_bd_pins /motor_control_0/pwm_out_right] [get_bd_ports MT_PWM_RIGHT]

create_bd_intf_port -mode Master -vlnv xilinx.com:interface:uart_rtl:1.0 BT_UART
connect_bd_intf_net [get_bd_intf_pins axi_uartlite_0/UART] [get_bd_intf_ports BT_UART]

create_bd_port -dir O US_TRIG
connect_bd_net [get_bd_pins /ultrasonic_control_0/trigger_out] [get_bd_ports US_TRIG]

create_bd_port -dir I US_ECHO
connect_bd_net [get_bd_pins /ultrasonic_control_0/echo_in] [get_bd_ports US_ECHO]

create_bd_port -dir I SP_LEFT
connect_bd_net [get_bd_pins /speed_control_0/left_sensor_in] [get_bd_ports SP_LEFT]

create_bd_port -dir I SP_RIGHT
connect_bd_net [get_bd_pins /speed_control_0/right_sensor_in] [get_bd_ports SP_RIGHT]


regenerate_bd_layout
validate_bd_design

# add hdl sources and xdc constraints to project
add_files -fileset sources_1 -norecurse $hdl_dir/design_1_wrapper.v
add_files -fileset constrs_1 -norecurse $constrs_dir/top_orig.xdc
update_compile_order -fileset sources_1
update_compile_order -fileset sim_1

set_property strategy Area_Explore [get_runs impl_1]


#launch_runs synth_1
#wait_on_run synth_1
#launch_runs impl_1
#wait_on_run impl_1
#launch_runs impl_1 -to_step write_bitstream
