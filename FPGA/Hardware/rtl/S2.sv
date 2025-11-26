module S2 
#(
parameter int tamanhoDados = 6 * $clog2(15) + 5,
parameter int TamanhoMalha = 15,
parameter addressTam = 2 * $clog2(TamanhoMalha),
parameter linear = $clog2(TamanhoMalha)
)(

	input clock,
	input reset,

	input [addressTam - 1:0] custoAcumulado,
	output reg [addressTam - 1:0] custoAcumuladoOut, 

	input [addressTam - 1:0] address,
	output reg [addressTam - 1:0] addressOut,

	input [1:0] direcao,
	output reg [1:0] direcaoOut,

	input [linear - 1 : 0] x,
	output reg [linear - 1 : 0] xOut,

	input [linear - 1 : 0] y,
	output reg [linear - 1 : 0] yOut,

	input [linear - 1 : 0] xOriginal,
	output reg [linear - 1 : 0] xOriginalOut,

	input [linear - 1 : 0] yOriginal,
	output reg [linear - 1 : 0] yOriginalOut,
	
	input aberto,
	output reg abertoOut,
	
	input [1:0] valor,
	output reg [1:0] valorOut,
	
	input candidato,
	output reg candidatoOut,
		
	input lastUse,
	output reg lastUseOut

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
		candidatoOut <= 0;
		lastUseOut <= 0;
	
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
		candidatoOut <= candidato;
		lastUseOut <= lastUse;
	
	end

end

endmodule