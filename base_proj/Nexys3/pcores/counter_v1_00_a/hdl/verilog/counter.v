//============================================================================//
//                                                                            //
//      Parameterize Counter                                                  //
//                                                                            //
//      Module name: counter                                                  //
//      Desc: parameterized counter, counts up/down in any increment          //
//      Date: Oct 2011                                                        //
//      Developer: Rurik Primiani & Wesley New                                //
//      Licence: GNU General Public License ver 3                             //
//      Notes:                                                                //
//                                                                            //
//============================================================================//

module counter #(
      //================================
      // Diagram positioning parameters
      //================================
      parameter BLOCK_NAME = "counter", // hierarchical block name
      parameter X  = 0,                 // x location within sub-block
      parameter Y  = 0,                 // y location within sub-block
      parameter DX = 0,                 // x length
      parameter DY = 0,                 // y lenghth
      
      //==============================
      // Top level block parameters
      //==============================
      parameter ARCHITECTURE = "BEHAVIORAL",     // BEHAVIORAL, VIRTEX5, VIRTEX6
      parameter DATA_WIDTH   = 8,                // number of bits in counter
      parameter COUNT_FROM   = 0,                // start with this number   
      parameter COUNT_TO     = 2^(DATA_WIDTH-1), // value to count to in CL case
      parameter STEP         = 1                 // negative or positive, sets direction
   ) (
      //===============
      // Input Ports
      //===============
      input clk,
      input en,
      input rst,
      
      //===============
      // Output Ports
      //===============
      output reg [DATA_WIDTH-1:0] o
   );

   //=======================================
   // Generate according to implementation
   //=======================================
   generate
      case (ARCHITECTURE)
         
         "BEHAVIORAL" : 
         begin
            // Synchronous logic
            always @(posedge clk)
            begin
               if (rst == 0 && o <= COUNT_TO)
               begin
     	            if (en == 1)
                  begin
     	               o <= o + STEP;
     	            end
     	         end
               else
               begin
     	            o <= COUNT_FROM;
               end // else: if(rst != 0)
            end
         end // case "BEHAVIORAL"
 
         "VIRTEX5" :
         begin
            // Instantiate V5 counter primitive
         end
     
         "VIRTEX6" :
         begin
            // Instantiate V6 counter primitive
         end
     
         default :
         begin
            // default case
         end

      endcase 
   endgenerate
endmodule