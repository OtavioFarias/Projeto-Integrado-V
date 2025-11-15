module S0 
#(
parameter int tamanhoDados = 6 * $clog2(15) + 5,
parameter int TamanhoMalha = 15,
parameter addressTam = 2 * $clog2(TamanhoMalha)
)(

	input clock,
	input reset,
	
	input proximoVizinho,
	output reg proximoVizinhoOut,
	
	input custoAcumulado,
	output reg custoAcumuladoOut,
	
	input direcao,
	output reg direcaoOut,
		
	input x,
	output reg xOut,
	
	input y,
	output reg yOut,
	
	input aberto,
	output reg abertoOut,
	
	input [1:0] valor,
	output reg [1:0] valorOut
	
);

always_ff @(posedge reset, posedge clock) begin

	if(reset) begin
		
		proximoVizinhoOut <= 0;
		custoAcumuladoOut <= 0;
		direcaoOut <= 0;
		xOut <= 0;
		yOut <= 0;
		abertoOut <= 0;
		valorOut <= 0;
	
	end
	else begin
	
		proximoVizinhoOut <= proximoVizinho;
		custoAcumuladoOut <= custoAcumulado;
		direcaoOut <= direcao;
		xOut <= x;
		yOut <= y;
		abertoOut <= aberto;
		valorOut <= valor;
	
	end

end



endmodule