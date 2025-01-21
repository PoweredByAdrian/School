-- uart_fsm.vhd: UART controller - finite state machine
-- Author(s): xhorva14
--
library ieee;
use ieee.std_logic_1164.all;

-------------------------------------------------
entity UART_FSM is
port(
   CLK:             in std_logic;
   RST:             in std_logic;
   DIN:             in std_logic;
   CLK_COUNT:       in std_logic_vector(4 downto 0);
   BIT_COUNT:       in std_logic_vector(3 downto 0);
   ENABLE_RECIEVE:  out std_logic;
   ENABLE_COUNT:    out std_logic;
   DATA_VALID:      out std_logic
   );
end entity UART_FSM;

-------------------------------------------------
architecture behavioral of UART_FSM is
type STATES is (WAITING_FOR_START_BIT, WAITING_FOR_FIRST_BIT, RECIEVING_DATA, WAITING_FOR_STOP_BIT, DATA_OUTPUT_VALID);
signal current_state : STATES := WAITING_FOR_START_BIT;
begin
   ENABLE_RECIEVE <= '1' when current_state = RECIEVING_DATA else '0';
   ENABLE_COUNT <= '1' when current_state = WAITING_FOR_FIRST_BIT or current_state = RECIEVING_DATA or current_state = WAITING_FOR_STOP_BIT else '0';
   DATA_VALID <= '1' when current_state = DATA_OUTPUT_VALID else '0';
   process (CLK) begin
      if rising_edge(CLK) then
         if RST = '1' then
            current_state <= WAITING_FOR_START_BIT;
         else
            case current_state is

            when WAITING_FOR_START_BIT => if DIN = '0' then
                                       current_state <= WAITING_FOR_FIRST_BIT;
                                    end if;
            when WAITING_FOR_FIRST_BIT => if CLK_COUNT = "10101" then --po 21 hodinovych signalov nacita prvy znak--
                                       current_state <= RECIEVING_DATA;
                                    end if;
            when RECIEVING_DATA => if BIT_COUNT = "1000" then --vsetky znaky nacitane--
                                       current_state <= WAITING_FOR_STOP_BIT;
                                    end if;
            when WAITING_FOR_STOP_BIT => if DIN = '1' then
                                       current_state <= DATA_OUTPUT_VALID;
                                       
                                    end if;
            when DATA_OUTPUT_VALID => current_state <= WAITING_FOR_START_BIT; --nastavenie valid na 1--
                                      
            when others         => null;
              
          end case;
         end if;
      end if;
   end process;
end behavioral;
