-- Copyright (C) 2018  Intel Corporation. All rights reserved.
-- Your use of Intel Corporation's design tools, logic functions 
-- and other software and tools, and its AMPP partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Intel Program License 
-- Subscription Agreement, the Intel Quartus Prime License Agreement,
-- the Intel FPGA IP License Agreement, or other applicable license
-- agreement, including, without limitation, that your use is for
-- the sole purpose of programming logic devices manufactured by
-- Intel and sold by Intel or its authorized distributors.  Please
-- refer to the applicable agreement for further details.

-- *****************************************************************************
-- This file contains a Vhdl test bench with test vectors .The test vectors     
-- are exported from a vector file in the Quartus Waveform Editor and apply to  
-- the top level entity of the current Quartus project .The user can use this   
-- testbench to simulate his design using a third-party simulation tool .       
-- *****************************************************************************
-- Generated on "04/12/2019 15:27:33"
                                                             
-- Vhdl Test Bench(with test vectors) for design  :          selfResetCounter
-- 
-- Simulation tool : 3rd Party
-- 

LIBRARY ieee;                                               
USE ieee.std_logic_1164.all;                                

ENTITY selfResetCounter_vhd_vec_tst IS
END selfResetCounter_vhd_vec_tst;
ARCHITECTURE selfResetCounter_arch OF selfResetCounter_vhd_vec_tst IS
-- constants                                                 
-- signals                                                   
SIGNAL clk : STD_LOGIC;
SIGNAL clkOut : STD_LOGIC;
SIGNAL count : STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL reset : STD_LOGIC;
SIGNAL top : STD_LOGIC_VECTOR(7 DOWNTO 0);
COMPONENT selfResetCounter
	PORT (
	clk : IN STD_LOGIC;
	clkOut : OUT STD_LOGIC;
	count : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
	reset : IN STD_LOGIC;
	top : IN STD_LOGIC_VECTOR(7 DOWNTO 0)
	);
END COMPONENT;
BEGIN
	i1 : selfResetCounter
	PORT MAP (
-- list connections between master ports and signals
	clk => clk,
	clkOut => clkOut,
	count => count,
	reset => reset,
	top => top
	);
-- top[7]
t_prcs_top_7: PROCESS
BEGIN
	top(7) <= '0';
WAIT;
END PROCESS t_prcs_top_7;
-- top[6]
t_prcs_top_6: PROCESS
BEGIN
	top(6) <= '0';
WAIT;
END PROCESS t_prcs_top_6;
-- top[5]
t_prcs_top_5: PROCESS
BEGIN
	top(5) <= '0';
WAIT;
END PROCESS t_prcs_top_5;
-- top[4]
t_prcs_top_4: PROCESS
BEGIN
	top(4) <= '0';
WAIT;
END PROCESS t_prcs_top_4;
-- top[3]
t_prcs_top_3: PROCESS
BEGIN
	top(3) <= '0';
WAIT;
END PROCESS t_prcs_top_3;
-- top[2]
t_prcs_top_2: PROCESS
BEGIN
	top(2) <= '1';
WAIT;
END PROCESS t_prcs_top_2;
-- top[1]
t_prcs_top_1: PROCESS
BEGIN
	top(1) <= '0';
WAIT;
END PROCESS t_prcs_top_1;
-- top[0]
t_prcs_top_0: PROCESS
BEGIN
	top(0) <= '0';
WAIT;
END PROCESS t_prcs_top_0;

-- reset
t_prcs_reset: PROCESS
BEGIN
	reset <= '0';
WAIT;
END PROCESS t_prcs_reset;

-- clk
t_prcs_clk: PROCESS
BEGIN
LOOP
	clk <= '0';
	WAIT FOR 20000 ps;
	clk <= '1';
	WAIT FOR 20000 ps;
	IF (NOW >= 1000000 ps) THEN WAIT; END IF;
END LOOP;
END PROCESS t_prcs_clk;
END selfResetCounter_arch;
