module S2 
#(
parameter int tamanhoDados = 6 * $clog2(15) + 5,
parameter int TamanhoMalha = 15,
parameter addressTam = 2 * $clog2(TamanhoMalha)
)(

	input clock,
	input reset,

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
	output reg yOriginalOut,
	
	input aberto,
	output reg abertoOut,
	
	input [1:0] valor,
	output reg [1:0] valorOut

);

always_ff @(posedge reset, posedge clock) begin

	if(reset) begin
	
		custoAcumuladoOut <= 0;
		addressOut <= 0;
		direcaoOut <= 0;
		xOut <= 0;
		yOut <= 0;
		xOriginalOut <= 0;
		yOriginalOut <= 0;
		abertoOut <= 0;
		valorOut <= 0;
	
	end
	else begin
	
		custoAcumuladoOut <= custoAcumulado;
		addressOut <= address;
		direcaoOut <= direcao;
		xOut <= x;
		yOut <= y;
		xOriginalOut <= xOriginal;
		yOriginalOut <= yOriginal;
		abertoOut <= aberto;
		valorOut <= valor;
	
	
	end

end

endmodule