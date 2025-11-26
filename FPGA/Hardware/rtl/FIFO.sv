module FIFO #(parameter DEPTH=8, DWIDTH=16)
(
    input               	rstn,               // Active low reset
                        	clk,                // Clock
                        	wr_en, 				// Write enable
                        	rd_en, 				// Read enable
    input      [DWIDTH-1:0] din, 				// Data written into FIFO
    output reg [DWIDTH-1:0] dout, 				// Data read from FIFO
    output              	empty, 				// FIFO is empty when high
                        	//full 				// FIFO is full when high
                        	
    output reg ready
);

  wire full;

  reg [$clog2(DEPTH)-1:0]   wptr;
  reg [$clog2(DEPTH)-1:0]   rptr;

  reg [DWIDTH-1 : 0]    fifo[DEPTH];

  always @ (posedge clk) begin
    if (!rstn) begin
      wptr <= 0;
    end else begin
      if (wr_en & !full) begin
        fifo[wptr] <= din;
        wptr <= wptr + 1;
      end
    end
  end
/*
  initial begin
    $monitor("[%0t] [FIFO] wr_en=%0b din=0x%0h rd_en=%0b dout=0x%0h empty=%0b full=%0b",
             $time, wr_en, din, rd_en, dout, empty, full);
  end
*/
  always @ (posedge clk) begin
    if (!rstn) begin
      rptr <= 0;
    end else begin
      if (rd_en & !empty && ready) begin
        dout <= fifo[rptr];
        rptr <= rptr + 1;
      end
    end
  end

  assign full  = (wptr + 1) == rptr;
  assign empty = wptr == rptr;
  
   always_ff @ (posedge clk) begin
    if (!rstn) begin
      ready <= 1;
    end else begin
	 
		if (rd_en & !empty && ready) begin
		
			ready <= 0;
		
		end
		else begin
	 
			if (rd_en == 0) begin
			  
				ready <= 1;

			end
		end
    end
  end
  
endmodule