#### SYSTEM
set_property IOSTANDARD LVCMOS33 [get_ports sys_clk]
set_property PACKAGE_PIN U22 [get_ports sys_clk]
set_property IOSTANDARD LVCMOS33 [get_ports sys_rst_n]
set_property PACKAGE_PIN P4 [get_ports sys_rst_n]

set_property PACKAGE_PIN T23 [get_ports {ledout_0[1]}]
set_property PACKAGE_PIN R23 [get_ports {ledout_0[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ledout_0[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {ledout_0[1]}]

set_property CLOCK_DEDICATED_ROUTE FALSE [get_nets design_1_i/octspi_top_0/inst/clkin_int]

set_property DRIVE 4 [get_ports {data_0[7]}]
set_property DRIVE 4 [get_ports {data_0[6]}]
set_property DRIVE 4 [get_ports {data_0[5]}]
set_property DRIVE 4 [get_ports {data_0[4]}]
set_property DRIVE 4 [get_ports {data_0[3]}]
set_property DRIVE 4 [get_ports {data_0[2]}]
set_property DRIVE 4 [get_ports {data_0[1]}]
set_property DRIVE 4 [get_ports {data_0[0]}]
set_property SLEW FAST [get_ports {data_0[7]}]
set_property SLEW FAST [get_ports {data_0[6]}]
set_property SLEW FAST [get_ports {data_0[5]}]
set_property SLEW FAST [get_ports {data_0[4]}]
set_property SLEW FAST [get_ports {data_0[3]}]
set_property SLEW FAST [get_ports {data_0[2]}]
set_property SLEW FAST [get_ports {data_0[1]}]
set_property SLEW FAST [get_ports {data_0[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {data_0[7]}]
set_property IOSTANDARD LVCMOS33 [get_ports {data_0[6]}]
set_property IOSTANDARD LVCMOS33 [get_ports {data_0[5]}]
set_property IOSTANDARD LVCMOS33 [get_ports {data_0[4]}]
set_property IOSTANDARD LVCMOS33 [get_ports {data_0[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {data_0[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {data_0[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {data_0[0]}]
set_property PACKAGE_PIN N2 [get_ports clkin_0]
set_property IOSTANDARD LVCMOS33 [get_ports clkin_0]
set_property PACKAGE_PIN P1 [get_ports {data_0[7]}]
set_property PACKAGE_PIN N1 [get_ports {data_0[6]}]
set_property PACKAGE_PIN M1 [get_ports {data_0[5]}]
set_property PACKAGE_PIN N3 [get_ports {data_0[4]}]
set_property PACKAGE_PIN L2 [get_ports {data_0[3]}]
set_property PACKAGE_PIN M2 [get_ports {data_0[2]}]
set_property PACKAGE_PIN L4 [get_ports {data_0[1]}]
set_property PACKAGE_PIN M4 [get_ports {data_0[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports dqs_0]
set_property IOSTANDARD LVCMOS33 [get_ports ncs_0]
set_property DRIVE 8 [get_ports dqs_0]
set_property SLEW FAST [get_ports dqs_0]
set_property PACKAGE_PIN L5 [get_ports dqs_0]
set_property PACKAGE_PIN R1 [get_ports ncs_0]


create_clock -period 16.667 -name spi_clk [get_ports clkin_0]

set_output_delay -clock spi_clk 10.000 [get_ports {data_0[7]}]
set_output_delay -clock spi_clk 10.000 [get_ports {data_0[6]}]
set_output_delay -clock spi_clk 10.000 [get_ports {data_0[5]}]
set_output_delay -clock spi_clk 10.000 [get_ports {data_0[4]}]
set_output_delay -clock spi_clk 10.000 [get_ports {data_0[3]}]
set_output_delay -clock spi_clk 10.000 [get_ports {data_0[2]}]
set_output_delay -clock spi_clk 10.000 [get_ports {data_0[1]}]
set_output_delay -clock spi_clk 10.000 [get_ports {data_0[0]}]

set_input_delay -clock spi_clk 10.000 [get_ports {data_0[7]}]
set_input_delay -clock spi_clk 10.000 [get_ports {data_0[6]}]
set_input_delay -clock spi_clk 10.000 [get_ports {data_0[5]}]
set_input_delay -clock spi_clk 10.000 [get_ports {data_0[4]}]
set_input_delay -clock spi_clk 10.000 [get_ports {data_0[3]}]
set_input_delay -clock spi_clk 10.000 [get_ports {data_0[2]}]
set_input_delay -clock spi_clk 10.000 [get_ports {data_0[1]}]
set_input_delay -clock spi_clk 10.000 [get_ports {data_0[0]}]

set_input_delay -clock spi_clk 10.000 [get_ports ncs_0]






create_debug_core u_ila_0 ila
set_property ALL_PROBE_SAME_MU true [get_debug_cores u_ila_0]
set_property ALL_PROBE_SAME_MU_CNT 1 [get_debug_cores u_ila_0]
set_property C_ADV_TRIGGER false [get_debug_cores u_ila_0]
set_property C_DATA_DEPTH 4096 [get_debug_cores u_ila_0]
set_property C_EN_STRG_QUAL false [get_debug_cores u_ila_0]
set_property C_INPUT_PIPE_STAGES 0 [get_debug_cores u_ila_0]
set_property C_TRIGIN_EN false [get_debug_cores u_ila_0]
set_property C_TRIGOUT_EN false [get_debug_cores u_ila_0]
set_property port_width 1 [get_debug_ports u_ila_0/clk]
connect_debug_port u_ila_0/clk [get_nets [list design_1_i/clk_wiz/inst/clk_out1]]
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe0]
set_property port_width 24 [get_debug_ports u_ila_0/probe0]
connect_debug_port u_ila_0/probe0 [get_nets [list {design_1_i/octspi_top_0/inst/len[0]} {design_1_i/octspi_top_0/inst/len[1]} {design_1_i/octspi_top_0/inst/len[2]} {design_1_i/octspi_top_0/inst/len[3]} {design_1_i/octspi_top_0/inst/len[4]} {design_1_i/octspi_top_0/inst/len[5]} {design_1_i/octspi_top_0/inst/len[6]} {design_1_i/octspi_top_0/inst/len[7]} {design_1_i/octspi_top_0/inst/len[8]} {design_1_i/octspi_top_0/inst/len[9]} {design_1_i/octspi_top_0/inst/len[10]} {design_1_i/octspi_top_0/inst/len[11]} {design_1_i/octspi_top_0/inst/len[12]} {design_1_i/octspi_top_0/inst/len[13]} {design_1_i/octspi_top_0/inst/len[14]} {design_1_i/octspi_top_0/inst/len[15]} {design_1_i/octspi_top_0/inst/len[16]} {design_1_i/octspi_top_0/inst/len[17]} {design_1_i/octspi_top_0/inst/len[18]} {design_1_i/octspi_top_0/inst/len[19]} {design_1_i/octspi_top_0/inst/len[20]} {design_1_i/octspi_top_0/inst/len[21]} {design_1_i/octspi_top_0/inst/len[22]} {design_1_i/octspi_top_0/inst/len[23]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe1]
set_property port_width 2 [get_debug_ports u_ila_0/probe1]
connect_debug_port u_ila_0/probe1 [get_nets [list {design_1_i/octspi_top_0/inst/dmy_cnt[0]} {design_1_i/octspi_top_0/inst/dmy_cnt[1]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe2]
set_property port_width 4 [get_debug_ports u_ila_0/probe2]
connect_debug_port u_ila_0/probe2 [get_nets [list {design_1_i/octspi_top_0/inst/cmd_cnt[0]} {design_1_i/octspi_top_0/inst/cmd_cnt[1]} {design_1_i/octspi_top_0/inst/cmd_cnt[2]} {design_1_i/octspi_top_0/inst/cmd_cnt[3]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe3]
set_property port_width 8 [get_debug_ports u_ila_0/probe3]
connect_debug_port u_ila_0/probe3 [get_nets [list {design_1_i/octspi_top_0/inst/data_cnt[0]} {design_1_i/octspi_top_0/inst/data_cnt[1]} {design_1_i/octspi_top_0/inst/data_cnt[2]} {design_1_i/octspi_top_0/inst/data_cnt[3]} {design_1_i/octspi_top_0/inst/data_cnt[4]} {design_1_i/octspi_top_0/inst/data_cnt[5]} {design_1_i/octspi_top_0/inst/data_cnt[6]} {design_1_i/octspi_top_0/inst/data_cnt[7]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe4]
set_property port_width 4 [get_debug_ports u_ila_0/probe4]
connect_debug_port u_ila_0/probe4 [get_nets [list {design_1_i/octspi_top_0/inst/cmd[0]} {design_1_i/octspi_top_0/inst/cmd[1]} {design_1_i/octspi_top_0/inst/cmd[2]} {design_1_i/octspi_top_0/inst/cmd[3]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe5]
set_property port_width 8 [get_debug_ports u_ila_0/probe5]
connect_debug_port u_ila_0/probe5 [get_nets [list {design_1_i/octspi_top_0/inst/data_i[0]} {design_1_i/octspi_top_0/inst/data_i[1]} {design_1_i/octspi_top_0/inst/data_i[2]} {design_1_i/octspi_top_0/inst/data_i[3]} {design_1_i/octspi_top_0/inst/data_i[4]} {design_1_i/octspi_top_0/inst/data_i[5]} {design_1_i/octspi_top_0/inst/data_i[6]} {design_1_i/octspi_top_0/inst/data_i[7]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe6]
set_property port_width 8 [get_debug_ports u_ila_0/probe6]
connect_debug_port u_ila_0/probe6 [get_nets [list {design_1_i/octspi_top_0/inst/data_d[0]} {design_1_i/octspi_top_0/inst/data_d[1]} {design_1_i/octspi_top_0/inst/data_d[2]} {design_1_i/octspi_top_0/inst/data_d[3]} {design_1_i/octspi_top_0/inst/data_d[4]} {design_1_i/octspi_top_0/inst/data_d[5]} {design_1_i/octspi_top_0/inst/data_d[6]} {design_1_i/octspi_top_0/inst/data_d[7]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe7]
set_property port_width 4 [get_debug_ports u_ila_0/probe7]
connect_debug_port u_ila_0/probe7 [get_nets [list {design_1_i/octspi_top_0/inst/size[0]} {design_1_i/octspi_top_0/inst/size[1]} {design_1_i/octspi_top_0/inst/size[2]} {design_1_i/octspi_top_0/inst/size[3]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe8]
set_property port_width 32 [get_debug_ports u_ila_0/probe8]
connect_debug_port u_ila_0/probe8 [get_nets [list {design_1_i/octspi_top_0/inst/address[0]} {design_1_i/octspi_top_0/inst/address[1]} {design_1_i/octspi_top_0/inst/address[2]} {design_1_i/octspi_top_0/inst/address[3]} {design_1_i/octspi_top_0/inst/address[4]} {design_1_i/octspi_top_0/inst/address[5]} {design_1_i/octspi_top_0/inst/address[6]} {design_1_i/octspi_top_0/inst/address[7]} {design_1_i/octspi_top_0/inst/address[8]} {design_1_i/octspi_top_0/inst/address[9]} {design_1_i/octspi_top_0/inst/address[10]} {design_1_i/octspi_top_0/inst/address[11]} {design_1_i/octspi_top_0/inst/address[12]} {design_1_i/octspi_top_0/inst/address[13]} {design_1_i/octspi_top_0/inst/address[14]} {design_1_i/octspi_top_0/inst/address[15]} {design_1_i/octspi_top_0/inst/address[16]} {design_1_i/octspi_top_0/inst/address[17]} {design_1_i/octspi_top_0/inst/address[18]} {design_1_i/octspi_top_0/inst/address[19]} {design_1_i/octspi_top_0/inst/address[20]} {design_1_i/octspi_top_0/inst/address[21]} {design_1_i/octspi_top_0/inst/address[22]} {design_1_i/octspi_top_0/inst/address[23]} {design_1_i/octspi_top_0/inst/address[24]} {design_1_i/octspi_top_0/inst/address[25]} {design_1_i/octspi_top_0/inst/address[26]} {design_1_i/octspi_top_0/inst/address[27]} {design_1_i/octspi_top_0/inst/address[28]} {design_1_i/octspi_top_0/inst/address[29]} {design_1_i/octspi_top_0/inst/address[30]} {design_1_i/octspi_top_0/inst/address[31]}]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe9]
set_property port_width 1 [get_debug_ports u_ila_0/probe9]
connect_debug_port u_ila_0/probe9 [get_nets [list design_1_i/octspi_top_0/inst/clkin_int]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe10]
set_property port_width 1 [get_debug_ports u_ila_0/probe10]
connect_debug_port u_ila_0/probe10 [get_nets [list design_1_i/octspi_top_0/inst/data_t]]
create_debug_port u_ila_0 probe
set_property PROBE_TYPE DATA_AND_TRIGGER [get_debug_ports u_ila_0/probe11]
set_property port_width 1 [get_debug_ports u_ila_0/probe11]
connect_debug_port u_ila_0/probe11 [get_nets [list design_1_i/octspi_top_0/inst/ncs]]
set_property C_CLK_INPUT_FREQ_HZ 300000000 [get_debug_cores dbg_hub]
set_property C_ENABLE_CLK_DIVIDER false [get_debug_cores dbg_hub]
set_property C_USER_SCAN_CHAIN 1 [get_debug_cores dbg_hub]
connect_debug_port dbg_hub/clk [get_nets u_ila_0_clk_out1]
