module vizinhoTop (

	input clock,
	input reset

);

Malha #(.TamanhoMalha(20*20), .tamanhoDados(8))
Malha (

    .clock(clock),
    .reset(reset)
/*
    .value(),
    .read(), 
    .write(),
    .address(),

    .outData()
  */  
);

FIFO#(.DEPTH(8), .DWIDTH(16)) 
filaHeap (

	.rstn(!reset),               // Active low reset
	.clk(clock)                // Clock
	/*
	wr_en, 				// Write enable
	rd_en, 				// Read enable
	din, 				// Data written into FIFO
	dout, 				// Data read from FIFO
	empty, 				// FIFO is empty when high
	full 				// FIFO is full when high
	*/
);

endmodule