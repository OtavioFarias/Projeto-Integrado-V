module Top (

	input clock,
	input reset

);

parameter tamanhoDados = 8 * $clog2(TamanhoMalha) + 3;
parameter TamanhoMalha = 15;

//NIOS
wire [tamanhoDados - 1 : 0] dataFromNIOS;
wire [1:0] direcaoFromNIOS;

//

vizinhoTop #(
	.TamanhoMalha(TamanhoMalha)
) vizinhos 
(

	.clock(clock),
	.reset(reset),
	
	.value(dataFromNIOS),
	.direcao(direcaoFromNIOS)

); 


Malha #(.TamanhoMalha(TamanhoMalha*TamanhoMalha), .tamanhoDados(tamanhoDados))
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