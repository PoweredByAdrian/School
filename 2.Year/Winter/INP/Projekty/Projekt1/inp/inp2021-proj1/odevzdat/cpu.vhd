-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2021 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- ram[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_WREN  : out std_logic;                    -- cteni z pameti (DATA_WREN='0') / zapis do pameti (DATA_WREN='1')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WREN musi byt '0'
   OUT_WREN : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

		signal pc_reg	: std_logic_vector(11 downto 0);
		signal pc_inc	: std_logic;
		signal pc_dec	: std_logic;	
		
		signal cnt_ptr : std_logic_vector(7 downto 0);
		signal cnt_dec : std_logic;
		signal cnt_inc : std_logic;
		
		signal ptr_inc : std_logic;
		signal ptr_dec : std_logic;
		signal ptr_reg : std_logic_vector(9 downto 0);
		
		type fsm_state is (
			s_init, s_fetch, s_decode, s_PTR_INC, s_PTR_DEC, s_VAL_INC, s_VAL_DEC, s_WHILE_START, s_WHILE_END, s_PUTCHAR, s_GETCHAR, s_BREAK, s_NULL,
			s_VAL_MX_INC, s_VAL_END_INC, s_VAL_MX_DEC, s_VAL_END_DEC, s_PUTCHAR_DONE, s_GETCHAR_DONE, s_WHILE_START_CHECK, s_WHILE_START_NEXT, 
			s_WHILE_START_END, s_WHILE_END_NEXT, s_WHILE_END_NEXT2, s_WHILE_END_NEXT3, s_WHILE_END_END, s_BREAK_NEXT, s_BREAK_END
			,s_WHILE_END_NEXT4);
		
		signal state	 : fsm_state := s_init;
		signal nstate	 : fsm_state;
		
		signal mx_sel: std_logic_vector (1 downto 0) := (others => '0');
		signal mx_out	 : std_logic_vector (7 downto 0) := (others => '0');
begin

	pc: process (RESET, CLK) is
	begin
		if RESET='1' then
			pc_reg <= (others=>'0');
		elsif rising_edge(CLK) then
			if pc_inc='1' then
				pc_reg <= pc_reg + 1;
			elsif pc_dec='1' then
				pc_reg <= pc_reg - 1;
			end if;
		end if;
	end process pc;
	CODE_ADDR <= pc_reg;
			
	ptr: process(RESET, CLK) is
	begin
		if RESET='1' then
			ptr_reg <= (others=>'0');
		elsif rising_edge(CLK) then
			if ptr_inc='1' then
				ptr_reg <= ptr_reg + 1;
			elsif ptr_dec='1' then
				ptr_reg <= ptr_reg - 1;
			end if;
		end if;	
	end process ptr;
	DATA_ADDR <= ptr_reg;	
	
	cnt: process (RESET, CLK) is
	begin
		if RESET='1' then
			cnt_ptr <= (others=>'0');
		elsif rising_edge(CLK) then
			if cnt_inc='1' then
				cnt_ptr <= cnt_ptr + 1;
			elsif cnt_dec='1' then
				cnt_ptr <= cnt_ptr - 1;
			end if;
		end if;
	end process;
	
	mx: process (RESET, CLK) is
	begin
		if RESET='1' then
			mx_out <= (others => '0');
		elsif rising_edge(CLK) then
			case mx_sel is
				when "00" =>
					mx_out <= IN_DATA;
				when "01" =>
					mx_out <= DATA_RDATA + "00000001";
				when "10" =>
					mx_out <= DATA_RDATA - "00000001";
				when "11" =>
					mx_out <= X"00";
				when others =>
					mx_out <= (others => '0');
			end case;
		end if;
	end process mx;
	DATA_WDATA <= mx_out;
	
	sl: process (CLK, RESET) is
		begin
			if RESET = '1' then
				state <= s_init;
			elsif rising_edge(CLK) then
				if EN = '1' then
					state <= nstate;
				end if;
			end if;
		end process sl;
	
	nsl: process(state, IN_VLD, OUT_BUSY, CODE_DATA, DATA_RDATA, cnt_ptr, mx_sel) is 
	begin
		nstate <= s_init;
		
		pc_inc <= '0';
		pc_dec <= '0';
		
		cnt_inc <= '0';
		cnt_dec <= '0';
		
		ptr_inc <= '0';
		ptr_dec <= '0';
		
		mx_sel <= "00";
		
		CODE_EN <= '1';
		
		IN_REQ <= '0';
		OUT_WREN<= '0';
		
		DATA_WREN <= '0';
		DATA_EN <= '0';
		
		case state is
			when s_init =>
				nstate <= s_fetch;
			when s_fetch =>
				nstate <= s_decode;
				CODE_EN <= '1';
			when s_decode =>
				case CODE_DATA is
					when X"3E" => 
						nstate <= s_PTR_INC;
					when X"3C" =>
						nstate <= s_PTR_DEC;
					when X"2B" => 
						nstate <= s_VAL_INC;
					when X"2D" =>
						nstate <= s_VAL_DEC;
					when X"5B" => 
						nstate <= s_WHILE_START;
					when X"5D" =>
						nstate <= s_WHILE_END;
					when X"2E" => 
						nstate <= s_PUTCHAR;
					when X"2C" =>
						nstate <= s_GETCHAR;
					when X"7E" => 
						nstate <= s_BREAK;
					when X"00" =>
						nstate <= s_NULL;
					when others =>
						pc_inc <= '1';
						nstate <= s_decode;
				end case;
			
			when s_PTR_INC =>
				pc_inc <= '1';
				ptr_inc <= '1';
				nstate <= s_fetch;
				
			when s_PTR_DEC =>
				pc_inc <= '1';
				ptr_dec <= '1';
				nstate <= s_fetch;
				
			when s_VAL_INC =>
				DATA_EN <= '1';
				DATA_WREN <= '0';
				nstate <= s_VAL_MX_INC;
			when s_VAL_MX_INC =>
				mx_sel <= "01";
				nstate <= s_VAL_END_INC;
			when s_VAL_END_INC =>
				DATA_EN <= '1';
				DATA_WREN <= '1';
				pc_inc <= '1';
				nstate <= s_fetch;
				
			when s_VAL_DEC =>
				DATA_EN <= '1';
				DATA_WREN <= '0';
				nstate <= s_VAL_MX_DEC;
			when s_VAL_MX_DEC =>
				mx_sel <= "10";
				nstate <= s_VAL_END_DEC;
			when s_VAL_END_DEC =>
				DATA_EN <= '1';
				DATA_WREN <= '1';
				pc_inc <= '1';
				nstate <= s_fetch;
				
			when s_PUTCHAR =>
				DATA_EN <= '1';
				DATA_WREN <= '0';
				nstate <= s_PUTCHAR_DONE;
			when s_PUTCHAR_DONE =>
				if OUT_BUSY = '1' then
					DATA_EN <= '1';
					DATA_WREN <= '0';
					nstate <= s_PUTCHAR_DONE;
				else
					OUT_DATA <= DATA_RDATA;
					OUT_WREN <= '1';
					pc_inc <= '1';
					nstate <= s_fetch;
				end if;
				
			when s_GETCHAR =>
				IN_REQ <= '1';
				mx_sel <= "00";
				nstate <= s_GETCHAR_DONE;
			when s_GETCHAR_DONE =>
				if IN_VLD /= '1' then 
					IN_REQ <= '1';
					mx_sel <= "00";
					nstate <= s_GETCHAR_DONE;
				else
					mx_sel <= "00";
					DATA_EN <= '1';
					DATA_WREN <= '1';
					pc_inc <= '1';
					nstate <= s_fetch;
					IN_REQ <= '0';
				end if;
				
			when s_WHILE_START =>
				pc_inc <= '1';
				DATA_EN <= '1';
				DATA_WREN <= '0';
				nstate <= s_WHILE_START_CHECK;
			when s_WHILE_START_CHECK =>
				if DATA_RDATA = "00000000" then
					nstate <= s_WHILE_START_NEXT;
				else
					nstate <= s_fetch;
				end if;
			when s_WHILE_START_NEXT =>
				if cnt_ptr = "00000000" then
					nstate <= s_fetch;
					cnt_inc <= '1';
				else
					nstate <= s_WHILE_START_END;
					CODE_EN <= '1';
				end if;
			when s_WHILE_START_END =>
				nstate <= s_WHILE_START_NEXT;
				pc_inc <= '1';
				if CODE_DATA = X"5B" then
					cnt_inc <= '1';
				else
					cnt_dec <= '1';
				end if;
				
			when s_WHILE_END =>
				DATA_EN <= '1';
				DATA_WREN <= '0';
				nstate <= s_WHILE_END_NEXT;
			when s_WHILE_END_NEXT =>
				if DATA_RDATA = "00000000" then
					nstate <= s_fetch;
					pc_inc <= '1';
				else
					nstate <= s_WHILE_END_NEXT2;
					cnt_inc <= '1';
					pc_dec <= '1';

				end if;
			when s_WHILE_END_NEXT2 =>
				if cnt_ptr = "00000000" then
					nstate <= s_fetch;
				else
					CODE_EN <= '1';
					nstate <= s_WHILE_END_NEXT3;
				end if;
			when s_WHILE_END_NEXT3 =>


				nstate <= s_WHILE_END_NEXT4;
			when s_WHILE_END_NEXT4 =>
				if CODE_DATA = X"5B" then
					cnt_dec <= '1';
				elsif CODE_DATA = X"5D" then
					cnt_inc <= '1';
				end if; 
				nstate <= s_WHILE_END_END;
			when s_WHILE_END_END =>
				nstate <= s_WHILE_END_NEXT2;
				if cnt_ptr = "00000000" then
					pc_inc <= '1';
				else
					pc_dec <= '1';
				end if;
					
			when s_BREAK =>
				pc_inc <= '1';
				cnt_inc <= '1';
				nstate <= s_BREAK_NEXT;
			when s_BREAK_NEXT =>
				if cnt_ptr /= "00000000" then
					CODE_EN <= '1';
					nstate <= s_BREAK_END;
				else
					nstate <= s_fetch;
				end if;
			when s_BREAK_END =>
				nstate <= s_BREAK_NEXT;
				pc_inc <= '1';
				if CODE_DATA = X"5B" then
					cnt_inc <= '1';
				elsif CODE_DATA = X"5D" then
					cnt_dec <= '1';
				end if;
				
			when s_NULL =>
				nstate <= s_NULL;
			
			end case;	
	end process nsl;
 -- zde dopiste vlastni VHDL kod

 -- pri tvorbe kodu reflektujte rady ze cviceni INP, zejmena mejte na pameti, ze 
 --   - nelze z vice procesu ovladat stejny signal,
 --   - je vhodne mit jeden proces pro popis jedne hardwarove komponenty, protoze pak
 --      - u synchronnich komponent obsahuje sensitivity list pouze CLK a RESET a 
 --      - u kombinacnich komponent obsahuje sensitivity list vsechny ctene signaly.
end behavioral;