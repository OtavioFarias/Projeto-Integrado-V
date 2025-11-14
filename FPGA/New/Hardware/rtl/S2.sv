module S2 (

	input clock,
	input reset,
	input flush,

	input custoAcumulado,
	output reg custoAcumuladoOut, 

	input address,
	output reg addressOut,

	input direcao,
	output reg direcaoOut,

	input x,
	output reg xOut,

	input y,
	output reg yOut,

	input xOriginal,
	output reg xOriginalOut,

	input yOriginal,
	output reg yOriginalOut

);

endmodule