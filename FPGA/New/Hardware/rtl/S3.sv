module S3 #(
parameter int tamanhoDados = 6 * $clog2(15) + 5,
parameter int TamanhoMalha = 15,
parameter addressTam = 2 * $clog2(TamanhoMalha)
)(

	input clock,
	input reset,

	input custoAcumulado,
	output reg custoAcumuladoOut, 

	input custoEstimado,
	output reg custoEstimadoOut, 

	input address,
	output reg addressOut,

	input direcao,
	output reg direcaoOut,

	input distanciaTotal,
	output reg distanciaTotalOut,
	
	input aberto,
	output reg abertoOut,
	
	input [1:0] valor,
	output reg [1:0] valorOut,
	
	input x,
	output reg xOut,

	input y,
	output reg yOut

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
	
	end

end

endmodule