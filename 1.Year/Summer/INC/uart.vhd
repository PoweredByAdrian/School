-- uart.vhd: UART controller - receiving part
-- Author(s): xhorva14
--
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

-------------------------------------------------
entity UART_RX is
port(	
  CLK: 	     in std_logic; 
	RST: 	     in std_logic;
	DIN: 	     in std_logic;
	DOUT: 	    out std_logic_vector(7 downto 0);
	DOUT_VLD: 	out std_logic
);
end UART_RX;  

-------------------------------------------------
architecture behavioral of UART_RX is
signal clk_count:      std_logic_vector(4 downto 0); --pocitadlo hodinoveho signalu--
signal bit_count:      std_logic_vector(3 downto 0); --pocitadlo precitanych bitov--
signal enable_recieve: std_logic;
signal enable_count:   std_logic;
signal data_valid:     std_logic;
begin
	FSM: entity work.UART_FSM(behavioral)
	port map (
		CLK 		          =>  CLK,
		RST			          =>  RST,
		DIN			          =>  DIN,
		CLK_COUNT		     =>  clk_count,
		BIT_COUNT		     =>  bit_count,
	  ENABLE_RECIEVE  =>  enable_recieve,
		ENABLE_COUNT    =>  enable_count,
		DATA_VALID      =>  data_valid
		);
	DOUT_VLD <= data_valid;
	process (CLK) begin
		if rising_edge(CLK) then
			if enable_count = '1' then 
				clk_count <= clk_count + 1;
			else
				clk_count <= "00000";
				bit_count <= "0000";
			end if;
			if enable_recieve = '1' then
				if clk_count(4) = '1' then
					clk_count <= "00001";
					case bit_count is
					when "0000" => DOUT(0) <= DIN;
					when "0001" => DOUT(1) <= DIN;
					when "0010" => DOUT(2) <= DIN;
					when "0011" => DOUT(3) <= DIN;
					when "0100" => DOUT(4) <= DIN;
					when "0101" => DOUT(5) <= DIN;
					when "0110" => DOUT(6) <= DIN;
					when "0111" => DOUT(7) <= DIN;
					when others => null;
					end case;
					bit_count <= bit_count + 1;
					if bit_count = "1000" then
						bit_count <= "0000";
					end if;
				end if;	
			end if;
		end if;
	end process;
end behavioral;
