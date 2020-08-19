-- fsm.vhd: Finite State Machine
-- Author: Mirka Kolarikova (xkolar76)
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (T0, T1, T2A, T2B, T3A, T3B, T4A, T4B, T5A, T5B, T6A, T6B, T7A, T7B, T8A, T8B, T9A, T9B, TKONEC, CHYBA, YESACCESS, NOACCESS, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= T0;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when T0 =>
      next_state <= T0;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(2) = '1') then
        next_state <= T1;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;

-- - - - - - - - - - - - - - - - - - - - - - -
   when T1 =>
      next_state <= T1;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(5) = '1') then
        next_state <= T2A;
      elsif (KEY(2) = '1') then
        next_state <= T2B;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when T2A =>
      next_state <= T2A;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(9) = '1') then
        next_state <= T3A;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;

 -- - - - - - - - - - - - - - - - - - - - - - -
   when T2B =>
      next_state <= T2B;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(2) = '1') then
        next_state <= T3B;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;

  -- - - - - - - - - - - - - - - - - - - - - - -
   when T3A =>
      next_state <= T3A;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(0) = '1') then
        next_state <= T4A;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when T3B =>
      next_state <= T3B;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(0) = '1') then
        next_state <= T4B;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when T4A =>
      next_state <= T4A;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(7) = '1') then
        next_state <= T5A;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
     -- - - - - - - - - - - - - - - - - - - - - - -
   when T4B =>
      next_state <= T4B;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(6) = '1') then
        next_state <= T5B;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
    -- - - - - - - - - - - - - - - - - - - - - - -
   when T5A =>
      next_state <= T5A;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(7) = '1') then
        next_state <= T6A;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
       -- - - - - - - - - - - - - - - - - - - - - - -
   when T5B =>
      next_state <= T5B;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(6) = '1') then
        next_state <= T6B;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when T6A =>
      next_state <= T6A;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(1) = '1') then
        next_state <= T7A;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when T6B =>
      next_state <= T6B;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(1) = '1') then
        next_state <= T7B;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
      -- - - - - - - - - - - - - - - - - - - - - - -
   when T7A =>
      next_state <= T7A;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(4) = '1') then
        next_state <= T8A;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when T7B =>
      next_state <= T7B;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(2) = '1') then
        next_state <= T8B;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
        -- - - - - - - - - - - - - - - - - - - - - - -
   when T8A =>
      next_state <= T8A;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(4) = '1') then
        next_state <= T9A;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when T8B =>
      next_state <= T8B;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(3) = '1') then
        next_state <= T9B;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when T9A =>
      next_state <= T9A;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(2) = '1') then
        next_state <= TKONEC;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when T9B =>
      next_state <= T9B;
      if (KEY(15) = '1') then
         next_state <= NOACCESS;
      elsif (KEY(6) = '1') then
        next_state <= TKONEC;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;
  -- - - - - - - - - - - - - - - - - - - - - - -
   when TKONEC =>
      next_state <= TKONEC;
      if (KEY(15) = '1') then
         next_state <= YESACCESS;
     elsif (KEY(14 downto 0) /= "000000000000000") then
        next_state <= CHYBA;
      end if;


  -- - - - - - - - - - - - - - - - - - - - - - -
   when YESACCESS =>
      next_state <= YESACCESS;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when NOACCESS =>
      next_state <= NOACCESS;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= T0; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when CHYBA =>
      next_state <= CHYBA;
      if (KEY(15) = '1') then
         next_state <= NOACCESS; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= T0;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is


  when FINISH =>
    if (KEY(15) = '1') then
      FSM_LCD_CLR    <= '1';
    end if;


   when YESACCESS =>

      FSM_LCD_WR     <= '1';
      FSM_MX_MEM     <= '1';
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';


   when NOACCESS =>

      FSM_LCD_WR     <= '1';
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';



   when others =>
    if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   end case;
end process output_logic;

end architecture behavioral;