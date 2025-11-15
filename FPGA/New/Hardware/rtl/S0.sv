module S0 
#(
parameter int tamanhoDados = 6 * $clog2(15) + 5,
parameter int TamanhoMalha = 15,
parameter addressTam = 2 * $clog2(TamanhoMalha),
parameter linear = $clog2(TamanhoMalha)
)(

	input clock,
	input reset,
	
	input [addressTam - 1:0] proximoVizinho,
	output reg [addressTam - 1:0] proximoVizinhoOut,
	
	input [addressTam - 1:0] custoAcumulado,
	output reg [addressTam - 1:0] custoAcumuladoOut,
	
	input [1:0] direcao,
	output reg [1:0] direcaoOut,
		
	input [linear - 1 : 0] x,
	output reg [linear - 1 : 0] xOut,
	
	input [linear - 1 : 0] y,
	output reg [linear - 1 : 0] yOut,
	
	input request,
	output reg requestOut
	
);

always_ff @(posedge reset, posedge clock) begin

	if(reset) begin
		
		proximoVizinhoOut <= 0;
		custoAcumuladoOut <= 0;
		direcaoOut <= 0;
		xOut <= 0;
		yOut <= 0;
		requestOut <= 0;
	
	end
	else begin
	
		proximoVizinhoOut <= proximoVizinho;
		custoAcumuladoOut <= custoAcumulado;
		direcaoOut <= direcao;
		xOut <= x;
		yOut <= y;
		requestOut <= request;
	
	end

end



endmodule