----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:45:40 02/09/2012 
-- Design Name: 
-- Module Name:    func1 - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity func1 is
    port ( x1_I : in  STD_LOGIC;
	x2_I : in  STD_LOGIC;
           z_O : out  STD_LOGIC);
end func1;

architecture Behavioral of func1 is

begin
	z_O <= x1_I xor x2_I; 


end Behavioral;

