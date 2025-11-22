module S3 #(
parameter int tamanhoDados = 6 * $clog2(15) + 5,
parameter int TamanhoMalha = 15,
parameter addressTam = 2 * $clog2(TamanhoMalha),
parameter linear = $clog2(TamanhoMalha)
)(

	input clock,
	input reset,

	input [addressTam - 1:0] custoAcumulado,
	output reg [addressTam - 1:0] custoAcumuladoOut, 

	input [addressTam - 1:0] custoEstimado,
	output reg [addressTam - 1:0] custoEstimadoOut, 

	input [addressTam - 1:0] address,
	output reg [addressTam - 1:0] addressOut,

	input [1:0] direcao,
	output reg [1:0] direcaoOut,

	input [TamanhoMalha*TamanhoMalha - 1 : 0] distanciaTotal,
	output reg [TamanhoMalha*TamanhoMalha - 1 : 0] distanciaTotalOut,
	
	input aberto,
	output reg abertoOut,
	
	input [1:0] valor,
	output reg [1:0] valorOut,
	
	input [linear - 1 : 0] x,
	output reg [linear - 1 : 0] xOut,

	input [linear - 1 : 0] y,
	output reg [linear - 1 : 0] yOut,
	
	input candidato,
	output reg candidatoOut,
	
	input [TamanhoMalha - 1 : 0] dx,
	output reg [TamanhoMalha - 1 : 0] dxOut,
	
	input [TamanhoMalha - 1 : 0] dy,
	output reg [TamanhoMalha - 1 : 0] dyOut,
		
	input lastUse,
	output reg lastUseOut

);

always_ff @(posedge reset, posedge clock) begin

	if(reset) begin
	
		custoAcumuladoOut <= 0;
		custoEstimadoOut <= 0;
		addressOut <= 0;
		direcaoOut <= 0;
		distanciaTotalOut <= 0;
		abertoOut <= 0;
		valorOut <= 0;
		xOut <= 0;
		yOut <= 0;
		candidatoOut <= 0;
		dxOut <= 0;
		dyOut <= 0;
		lastUseOut <= 0;
	
	end
	else begin
	
		custoAcumuladoOut <= custoAcumulado;
		custoEstimadoOut <= custoEstimado;
		addressOut <= address;
		direcaoOut <= direcao;
		distanciaTotalOut <= distanciaTotal;
		abertoOut <= aberto;
		valorOut <= valor;
		xOut <= x;
		yOut <= y;
		candidatoOut <= candidato;
		dyOut <= dy;
		dxOut <= dx;
		lastUseOut <= lastUse;
		
	end

end

endmodule