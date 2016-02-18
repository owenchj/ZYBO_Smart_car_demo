-- ==============================================================
-- RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
-- Version: 2015.2
-- Copyright (C) 2015 Xilinx Inc. All rights reserved.
-- 
-- ===========================================================

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity sobel_Block_Mat_exit49_proc88 is
port (
    ap_clk : IN STD_LOGIC;
    ap_rst : IN STD_LOGIC;
    ap_start : IN STD_LOGIC;
    ap_done : OUT STD_LOGIC;
    ap_continue : IN STD_LOGIC;
    ap_idle : OUT STD_LOGIC;
    ap_ready : OUT STD_LOGIC;
    rows : IN STD_LOGIC_VECTOR (31 downto 0);
    cols : IN STD_LOGIC_VECTOR (31 downto 0);
    C_XR2C0 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_YR0C0 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_YR0C2 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_YR1C0 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_YR1C2 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_YR2C0 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_YR2C1 : IN STD_LOGIC_VECTOR (31 downto 0);
    c_high_thresh : IN STD_LOGIC_VECTOR (31 downto 0);
    C_YR1C1 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_XR1C2 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_YR0C1 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_XR2C2 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_XR0C0 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_XR0C1 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_XR0C2 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_XR1C0 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_XR2C1 : IN STD_LOGIC_VECTOR (31 downto 0);
    C_XR1C1 : IN STD_LOGIC_VECTOR (31 downto 0);
    c_low_thresh : IN STD_LOGIC_VECTOR (31 downto 0);
    c_invert : IN STD_LOGIC_VECTOR (31 downto 0);
    empty_3 : IN STD_LOGIC_VECTOR (9 downto 0);
    empty_4 : IN STD_LOGIC_VECTOR (8 downto 0);
    empty : IN STD_LOGIC_VECTOR (7 downto 0);
    img_0_rows_V_out_din : OUT STD_LOGIC_VECTOR (11 downto 0);
    img_0_rows_V_out_full_n : IN STD_LOGIC;
    img_0_rows_V_out_write : OUT STD_LOGIC;
    img_0_rows_V_out1_din : OUT STD_LOGIC_VECTOR (11 downto 0);
    img_0_rows_V_out1_full_n : IN STD_LOGIC;
    img_0_rows_V_out1_write : OUT STD_LOGIC;
    img_0_cols_V_out_din : OUT STD_LOGIC_VECTOR (11 downto 0);
    img_0_cols_V_out_full_n : IN STD_LOGIC;
    img_0_cols_V_out_write : OUT STD_LOGIC;
    img_0_cols_V_out2_din : OUT STD_LOGIC_VECTOR (11 downto 0);
    img_0_cols_V_out2_full_n : IN STD_LOGIC;
    img_0_cols_V_out2_write : OUT STD_LOGIC;
    C_XR2C0_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_XR2C0_out_full_n : IN STD_LOGIC;
    C_XR2C0_out_write : OUT STD_LOGIC;
    C_YR0C0_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_YR0C0_out_full_n : IN STD_LOGIC;
    C_YR0C0_out_write : OUT STD_LOGIC;
    C_YR0C2_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_YR0C2_out_full_n : IN STD_LOGIC;
    C_YR0C2_out_write : OUT STD_LOGIC;
    C_YR1C0_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_YR1C0_out_full_n : IN STD_LOGIC;
    C_YR1C0_out_write : OUT STD_LOGIC;
    C_YR1C2_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_YR1C2_out_full_n : IN STD_LOGIC;
    C_YR1C2_out_write : OUT STD_LOGIC;
    C_YR2C0_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_YR2C0_out_full_n : IN STD_LOGIC;
    C_YR2C0_out_write : OUT STD_LOGIC;
    C_YR2C1_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_YR2C1_out_full_n : IN STD_LOGIC;
    C_YR2C1_out_write : OUT STD_LOGIC;
    c_high_thresh_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    c_high_thresh_out_full_n : IN STD_LOGIC;
    c_high_thresh_out_write : OUT STD_LOGIC;
    C_YR1C1_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_YR1C1_out_full_n : IN STD_LOGIC;
    C_YR1C1_out_write : OUT STD_LOGIC;
    C_XR1C2_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_XR1C2_out_full_n : IN STD_LOGIC;
    C_XR1C2_out_write : OUT STD_LOGIC;
    C_YR0C1_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_YR0C1_out_full_n : IN STD_LOGIC;
    C_YR0C1_out_write : OUT STD_LOGIC;
    C_XR2C2_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_XR2C2_out_full_n : IN STD_LOGIC;
    C_XR2C2_out_write : OUT STD_LOGIC;
    C_XR0C0_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_XR0C0_out_full_n : IN STD_LOGIC;
    C_XR0C0_out_write : OUT STD_LOGIC;
    C_XR0C1_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_XR0C1_out_full_n : IN STD_LOGIC;
    C_XR0C1_out_write : OUT STD_LOGIC;
    C_XR0C2_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_XR0C2_out_full_n : IN STD_LOGIC;
    C_XR0C2_out_write : OUT STD_LOGIC;
    C_XR1C0_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_XR1C0_out_full_n : IN STD_LOGIC;
    C_XR1C0_out_write : OUT STD_LOGIC;
    C_XR2C1_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_XR2C1_out_full_n : IN STD_LOGIC;
    C_XR2C1_out_write : OUT STD_LOGIC;
    C_XR1C1_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    C_XR1C1_out_full_n : IN STD_LOGIC;
    C_XR1C1_out_write : OUT STD_LOGIC;
    c_low_thresh_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    c_low_thresh_out_full_n : IN STD_LOGIC;
    c_low_thresh_out_write : OUT STD_LOGIC;
    c_invert_out_din : OUT STD_LOGIC_VECTOR (31 downto 0);
    c_invert_out_full_n : IN STD_LOGIC;
    c_invert_out_write : OUT STD_LOGIC;
    p_out_din : OUT STD_LOGIC_VECTOR (9 downto 0);
    p_out_full_n : IN STD_LOGIC;
    p_out_write : OUT STD_LOGIC;
    p_out1_din : OUT STD_LOGIC_VECTOR (8 downto 0);
    p_out1_full_n : IN STD_LOGIC;
    p_out1_write : OUT STD_LOGIC;
    p_out2_din : OUT STD_LOGIC_VECTOR (7 downto 0);
    p_out2_full_n : IN STD_LOGIC;
    p_out2_write : OUT STD_LOGIC;
    ap_return_0 : OUT STD_LOGIC_VECTOR (11 downto 0);
    ap_return_1 : OUT STD_LOGIC_VECTOR (11 downto 0);
    ap_return_2 : OUT STD_LOGIC_VECTOR (11 downto 0);
    ap_return_3 : OUT STD_LOGIC_VECTOR (11 downto 0) );
end;


architecture behav of sobel_Block_Mat_exit49_proc88 is 
    constant ap_const_logic_1 : STD_LOGIC := '1';
    constant ap_const_logic_0 : STD_LOGIC := '0';
    constant ap_ST_st1_fsm_0 : STD_LOGIC_VECTOR (0 downto 0) := "1";
    constant ap_const_lv32_0 : STD_LOGIC_VECTOR (31 downto 0) := "00000000000000000000000000000000";
    constant ap_const_lv1_1 : STD_LOGIC_VECTOR (0 downto 0) := "1";
    constant ap_const_lv12_0 : STD_LOGIC_VECTOR (11 downto 0) := "000000000000";

    signal ap_done_reg : STD_LOGIC := '0';
    signal ap_CS_fsm : STD_LOGIC_VECTOR (0 downto 0) := "1";
    attribute fsm_encoding : string;
    attribute fsm_encoding of ap_CS_fsm : signal is "none";
    signal ap_sig_cseq_ST_st1_fsm_0 : STD_LOGIC;
    signal ap_sig_bdd_20 : BOOLEAN;
    signal ap_sig_bdd_223 : BOOLEAN;
    signal img_0_rows_V_fu_492_p1 : STD_LOGIC_VECTOR (11 downto 0);
    signal img_0_cols_V_fu_498_p1 : STD_LOGIC_VECTOR (11 downto 0);
    signal ap_return_0_preg : STD_LOGIC_VECTOR (11 downto 0) := "000000000000";
    signal ap_return_1_preg : STD_LOGIC_VECTOR (11 downto 0) := "000000000000";
    signal ap_return_2_preg : STD_LOGIC_VECTOR (11 downto 0) := "000000000000";
    signal ap_return_3_preg : STD_LOGIC_VECTOR (11 downto 0) := "000000000000";
    signal ap_NS_fsm : STD_LOGIC_VECTOR (0 downto 0);


begin




    -- the current state (ap_CS_fsm) of the state machine. --
    ap_CS_fsm_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_CS_fsm <= ap_ST_st1_fsm_0;
            else
                ap_CS_fsm <= ap_NS_fsm;
            end if;
        end if;
    end process;


    -- ap_done_reg assign process. --
    ap_done_reg_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_done_reg <= ap_const_logic_0;
            else
                if ((ap_const_logic_1 = ap_continue)) then 
                    ap_done_reg <= ap_const_logic_0;
                elsif (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
                    ap_done_reg <= ap_const_logic_1;
                end if; 
            end if;
        end if;
    end process;


    -- ap_return_0_preg assign process. --
    ap_return_0_preg_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_return_0_preg <= ap_const_lv12_0;
            else
                if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
                    ap_return_0_preg <= img_0_rows_V_fu_492_p1;
                end if; 
            end if;
        end if;
    end process;


    -- ap_return_1_preg assign process. --
    ap_return_1_preg_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_return_1_preg <= ap_const_lv12_0;
            else
                if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
                    ap_return_1_preg <= img_0_cols_V_fu_498_p1;
                end if; 
            end if;
        end if;
    end process;


    -- ap_return_2_preg assign process. --
    ap_return_2_preg_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_return_2_preg <= ap_const_lv12_0;
            else
                if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
                    ap_return_2_preg <= img_0_rows_V_fu_492_p1;
                end if; 
            end if;
        end if;
    end process;


    -- ap_return_3_preg assign process. --
    ap_return_3_preg_assign_proc : process(ap_clk)
    begin
        if (ap_clk'event and ap_clk =  '1') then
            if (ap_rst = '1') then
                ap_return_3_preg <= ap_const_lv12_0;
            else
                if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
                    ap_return_3_preg <= img_0_cols_V_fu_498_p1;
                end if; 
            end if;
        end if;
    end process;


    -- the next state (ap_NS_fsm) of the state machine. --
    ap_NS_fsm_assign_proc : process (ap_CS_fsm, ap_sig_bdd_223)
    begin
        case ap_CS_fsm is
            when ap_ST_st1_fsm_0 => 
                ap_NS_fsm <= ap_ST_st1_fsm_0;
            when others =>  
                ap_NS_fsm <= "X";
        end case;
    end process;
    C_XR0C0_out_din <= C_XR0C0;

    -- C_XR0C0_out_write assign process. --
    C_XR0C0_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_XR0C0_out_write <= ap_const_logic_1;
        else 
            C_XR0C0_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_XR0C1_out_din <= C_XR0C1;

    -- C_XR0C1_out_write assign process. --
    C_XR0C1_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_XR0C1_out_write <= ap_const_logic_1;
        else 
            C_XR0C1_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_XR0C2_out_din <= C_XR0C2;

    -- C_XR0C2_out_write assign process. --
    C_XR0C2_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_XR0C2_out_write <= ap_const_logic_1;
        else 
            C_XR0C2_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_XR1C0_out_din <= C_XR1C0;

    -- C_XR1C0_out_write assign process. --
    C_XR1C0_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_XR1C0_out_write <= ap_const_logic_1;
        else 
            C_XR1C0_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_XR1C1_out_din <= C_XR1C1;

    -- C_XR1C1_out_write assign process. --
    C_XR1C1_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_XR1C1_out_write <= ap_const_logic_1;
        else 
            C_XR1C1_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_XR1C2_out_din <= C_XR1C2;

    -- C_XR1C2_out_write assign process. --
    C_XR1C2_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_XR1C2_out_write <= ap_const_logic_1;
        else 
            C_XR1C2_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_XR2C0_out_din <= C_XR2C0;

    -- C_XR2C0_out_write assign process. --
    C_XR2C0_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_XR2C0_out_write <= ap_const_logic_1;
        else 
            C_XR2C0_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_XR2C1_out_din <= C_XR2C1;

    -- C_XR2C1_out_write assign process. --
    C_XR2C1_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_XR2C1_out_write <= ap_const_logic_1;
        else 
            C_XR2C1_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_XR2C2_out_din <= C_XR2C2;

    -- C_XR2C2_out_write assign process. --
    C_XR2C2_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_XR2C2_out_write <= ap_const_logic_1;
        else 
            C_XR2C2_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_YR0C0_out_din <= C_YR0C0;

    -- C_YR0C0_out_write assign process. --
    C_YR0C0_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_YR0C0_out_write <= ap_const_logic_1;
        else 
            C_YR0C0_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_YR0C1_out_din <= C_YR0C1;

    -- C_YR0C1_out_write assign process. --
    C_YR0C1_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_YR0C1_out_write <= ap_const_logic_1;
        else 
            C_YR0C1_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_YR0C2_out_din <= C_YR0C2;

    -- C_YR0C2_out_write assign process. --
    C_YR0C2_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_YR0C2_out_write <= ap_const_logic_1;
        else 
            C_YR0C2_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_YR1C0_out_din <= C_YR1C0;

    -- C_YR1C0_out_write assign process. --
    C_YR1C0_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_YR1C0_out_write <= ap_const_logic_1;
        else 
            C_YR1C0_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_YR1C1_out_din <= C_YR1C1;

    -- C_YR1C1_out_write assign process. --
    C_YR1C1_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_YR1C1_out_write <= ap_const_logic_1;
        else 
            C_YR1C1_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_YR1C2_out_din <= C_YR1C2;

    -- C_YR1C2_out_write assign process. --
    C_YR1C2_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_YR1C2_out_write <= ap_const_logic_1;
        else 
            C_YR1C2_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_YR2C0_out_din <= C_YR2C0;

    -- C_YR2C0_out_write assign process. --
    C_YR2C0_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_YR2C0_out_write <= ap_const_logic_1;
        else 
            C_YR2C0_out_write <= ap_const_logic_0;
        end if; 
    end process;

    C_YR2C1_out_din <= C_YR2C1;

    -- C_YR2C1_out_write assign process. --
    C_YR2C1_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            C_YR2C1_out_write <= ap_const_logic_1;
        else 
            C_YR2C1_out_write <= ap_const_logic_0;
        end if; 
    end process;


    -- ap_done assign process. --
    ap_done_assign_proc : process(ap_done_reg, ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_done_reg) or ((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223)))) then 
            ap_done <= ap_const_logic_1;
        else 
            ap_done <= ap_const_logic_0;
        end if; 
    end process;


    -- ap_idle assign process. --
    ap_idle_assign_proc : process(ap_start, ap_sig_cseq_ST_st1_fsm_0)
    begin
        if ((not((ap_const_logic_1 = ap_start)) and (ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0))) then 
            ap_idle <= ap_const_logic_1;
        else 
            ap_idle <= ap_const_logic_0;
        end if; 
    end process;


    -- ap_ready assign process. --
    ap_ready_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            ap_ready <= ap_const_logic_1;
        else 
            ap_ready <= ap_const_logic_0;
        end if; 
    end process;


    -- ap_return_0 assign process. --
    ap_return_0_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223, img_0_rows_V_fu_492_p1, ap_return_0_preg)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            ap_return_0 <= img_0_rows_V_fu_492_p1;
        else 
            ap_return_0 <= ap_return_0_preg;
        end if; 
    end process;


    -- ap_return_1 assign process. --
    ap_return_1_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223, img_0_cols_V_fu_498_p1, ap_return_1_preg)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            ap_return_1 <= img_0_cols_V_fu_498_p1;
        else 
            ap_return_1 <= ap_return_1_preg;
        end if; 
    end process;


    -- ap_return_2 assign process. --
    ap_return_2_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223, img_0_rows_V_fu_492_p1, ap_return_2_preg)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            ap_return_2 <= img_0_rows_V_fu_492_p1;
        else 
            ap_return_2 <= ap_return_2_preg;
        end if; 
    end process;


    -- ap_return_3 assign process. --
    ap_return_3_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223, img_0_cols_V_fu_498_p1, ap_return_3_preg)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            ap_return_3 <= img_0_cols_V_fu_498_p1;
        else 
            ap_return_3 <= ap_return_3_preg;
        end if; 
    end process;


    -- ap_sig_bdd_20 assign process. --
    ap_sig_bdd_20_assign_proc : process(ap_CS_fsm)
    begin
                ap_sig_bdd_20 <= (ap_CS_fsm(0 downto 0) = ap_const_lv1_1);
    end process;


    -- ap_sig_bdd_223 assign process. --
    ap_sig_bdd_223_assign_proc : process(ap_start, ap_done_reg, img_0_rows_V_out_full_n, img_0_rows_V_out1_full_n, img_0_cols_V_out_full_n, img_0_cols_V_out2_full_n, C_XR2C0_out_full_n, C_YR0C0_out_full_n, C_YR0C2_out_full_n, C_YR1C0_out_full_n, C_YR1C2_out_full_n, C_YR2C0_out_full_n, C_YR2C1_out_full_n, c_high_thresh_out_full_n, C_YR1C1_out_full_n, C_XR1C2_out_full_n, C_YR0C1_out_full_n, C_XR2C2_out_full_n, C_XR0C0_out_full_n, C_XR0C1_out_full_n, C_XR0C2_out_full_n, C_XR1C0_out_full_n, C_XR2C1_out_full_n, C_XR1C1_out_full_n, c_low_thresh_out_full_n, c_invert_out_full_n, p_out_full_n, p_out1_full_n, p_out2_full_n)
    begin
                ap_sig_bdd_223 <= ((C_XR2C0_out_full_n = ap_const_logic_0) or (C_YR0C0_out_full_n = ap_const_logic_0) or (C_YR0C2_out_full_n = ap_const_logic_0) or (C_YR1C0_out_full_n = ap_const_logic_0) or (C_YR1C2_out_full_n = ap_const_logic_0) or (C_YR2C0_out_full_n = ap_const_logic_0) or (C_YR2C1_out_full_n = ap_const_logic_0) or (c_high_thresh_out_full_n = ap_const_logic_0) or (C_YR1C1_out_full_n = ap_const_logic_0) or (C_XR1C2_out_full_n = ap_const_logic_0) or (C_YR0C1_out_full_n = ap_const_logic_0) or (C_XR2C2_out_full_n = ap_const_logic_0) or (C_XR0C0_out_full_n = ap_const_logic_0) or (C_XR0C1_out_full_n = ap_const_logic_0) or (C_XR0C2_out_full_n = ap_const_logic_0) or (C_XR1C0_out_full_n = ap_const_logic_0) or (C_XR2C1_out_full_n = ap_const_logic_0) or (C_XR1C1_out_full_n = ap_const_logic_0) or (c_low_thresh_out_full_n = ap_const_logic_0) or (c_invert_out_full_n = ap_const_logic_0) or (p_out_full_n = ap_const_logic_0) or (p_out1_full_n = ap_const_logic_0) or (p_out2_full_n = ap_const_logic_0) or (img_0_rows_V_out_full_n = ap_const_logic_0) or (img_0_rows_V_out1_full_n = ap_const_logic_0) or (img_0_cols_V_out_full_n = ap_const_logic_0) or (img_0_cols_V_out2_full_n = ap_const_logic_0) or (ap_start = ap_const_logic_0) or (ap_done_reg = ap_const_logic_1));
    end process;


    -- ap_sig_cseq_ST_st1_fsm_0 assign process. --
    ap_sig_cseq_ST_st1_fsm_0_assign_proc : process(ap_sig_bdd_20)
    begin
        if (ap_sig_bdd_20) then 
            ap_sig_cseq_ST_st1_fsm_0 <= ap_const_logic_1;
        else 
            ap_sig_cseq_ST_st1_fsm_0 <= ap_const_logic_0;
        end if; 
    end process;

    c_high_thresh_out_din <= c_high_thresh;

    -- c_high_thresh_out_write assign process. --
    c_high_thresh_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            c_high_thresh_out_write <= ap_const_logic_1;
        else 
            c_high_thresh_out_write <= ap_const_logic_0;
        end if; 
    end process;

    c_invert_out_din <= c_invert;

    -- c_invert_out_write assign process. --
    c_invert_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            c_invert_out_write <= ap_const_logic_1;
        else 
            c_invert_out_write <= ap_const_logic_0;
        end if; 
    end process;

    c_low_thresh_out_din <= c_low_thresh;

    -- c_low_thresh_out_write assign process. --
    c_low_thresh_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            c_low_thresh_out_write <= ap_const_logic_1;
        else 
            c_low_thresh_out_write <= ap_const_logic_0;
        end if; 
    end process;

    img_0_cols_V_fu_498_p1 <= cols(12 - 1 downto 0);
    img_0_cols_V_out2_din <= img_0_cols_V_fu_498_p1;

    -- img_0_cols_V_out2_write assign process. --
    img_0_cols_V_out2_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            img_0_cols_V_out2_write <= ap_const_logic_1;
        else 
            img_0_cols_V_out2_write <= ap_const_logic_0;
        end if; 
    end process;

    img_0_cols_V_out_din <= img_0_cols_V_fu_498_p1;

    -- img_0_cols_V_out_write assign process. --
    img_0_cols_V_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            img_0_cols_V_out_write <= ap_const_logic_1;
        else 
            img_0_cols_V_out_write <= ap_const_logic_0;
        end if; 
    end process;

    img_0_rows_V_fu_492_p1 <= rows(12 - 1 downto 0);
    img_0_rows_V_out1_din <= img_0_rows_V_fu_492_p1;

    -- img_0_rows_V_out1_write assign process. --
    img_0_rows_V_out1_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            img_0_rows_V_out1_write <= ap_const_logic_1;
        else 
            img_0_rows_V_out1_write <= ap_const_logic_0;
        end if; 
    end process;

    img_0_rows_V_out_din <= img_0_rows_V_fu_492_p1;

    -- img_0_rows_V_out_write assign process. --
    img_0_rows_V_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            img_0_rows_V_out_write <= ap_const_logic_1;
        else 
            img_0_rows_V_out_write <= ap_const_logic_0;
        end if; 
    end process;

    p_out1_din <= empty_4;

    -- p_out1_write assign process. --
    p_out1_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            p_out1_write <= ap_const_logic_1;
        else 
            p_out1_write <= ap_const_logic_0;
        end if; 
    end process;

    p_out2_din <= empty;

    -- p_out2_write assign process. --
    p_out2_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            p_out2_write <= ap_const_logic_1;
        else 
            p_out2_write <= ap_const_logic_0;
        end if; 
    end process;

    p_out_din <= empty_3;

    -- p_out_write assign process. --
    p_out_write_assign_proc : process(ap_sig_cseq_ST_st1_fsm_0, ap_sig_bdd_223)
    begin
        if (((ap_const_logic_1 = ap_sig_cseq_ST_st1_fsm_0) and not(ap_sig_bdd_223))) then 
            p_out_write <= ap_const_logic_1;
        else 
            p_out_write <= ap_const_logic_0;
        end if; 
    end process;

end behav;