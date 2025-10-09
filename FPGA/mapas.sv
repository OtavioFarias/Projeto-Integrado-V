module mapas #(parameter int TamanhoMalha = 20, parameter int tamanhoDistancia = 8 /* em bits*/)(

	input clock,
	input reset,
	
	input [tamanhoDistancia] posicaoAtualnoEixoX,
	input [tamanhoDistancia] posicaoAtualnoEixoY,
	input direcaoAtual, // 0 - horizontal, 1 - vertical
	
	input [tamanhoDistancia] distanciaFrente,
	input [tamanhoDistancia] distanciaDireita,
	input [tamanhoDistancia] distanciaEsquerda,
	
	input novoDado,
	
	
	output reg [1:0] malha[TamanhoMalha][TamanhoMalha], //x,y - 00 - desconhecido, 01 - livre, 10 - ocupado
	output reg operacaoFinalizada // valor 1 se operação finalizada e disponível para receber novas requisições

);

int x,y;

localparam marcarVertical = 2'b00,
		   marcarHorizontal = 2'b01,
		   IDLE = 2'b10;

reg marcarDireita;
reg marcarEsquerda;

reg [tamanhoDistancia] posicaoDireita;
reg [tamanhoDistancia] posicaoEsquerda;

reg [tamanhoDistancia] diretaAcabou;
reg [tamanhoDistancia] esquerdaAcabou;

reg [1:0] stage;

always_ff @(posedge clock, posedge reset) begin

	if(reset) begin
	
		 for (x = 0; x < TamanhoMalha; x++) begin
		 	for (y = 0; y < TamanhoMalha; y++) begin
		 
		 		malha[x][y] <= 2'b00;
		 	
		 	end
		 end
		 
		 operacaoFinalizada <= 1'b1;
		 stage <= IDLE;
		 
		marcarDireita <= 0;
		marcarEsquerda <= 0;
		
		diretaAcabou <= 1'b0;
		esquerdaAcabou <= 1'b0;
		 
		 
	end
	else begin
		
		if(novoDado && stage == IDLE) begin
		
			operacaoFinalizada <= 1'b0;
			
			// mandar dados para a unidade responsável por marcar
			// aqui os conceitos de direita e esquerda são em relação a posição atual do carrinho
			if(direcaoAtual) begin // vertical be

				marcarDireita  <=  posicaoAtualnoEixoY + distanciaDireita - 1;
				marcarEsquerda <= posicaoAtualnoEixoY - distanciaEsquerda + 1;
				
				// posições atuais da célula a ser marcada, começa na posição atual do carrinho e direciona para até a célula destino
				posicaoDireita <= posicaoAtualnoEixoY;
				posicaoEsquerda <= posicaoAtualnoEixoY;
				
				stage <= marcarVertical;
				
				malha[posicaoAtualnoEixoX + distanciaDireita][posicaoAtualnoEixoY] <= 2'b10;
				malha[posicaoAtualnoEixoX + distanciaEsquerda][posicaoAtualnoEixoY] <= 2'b10;
			
			end
			else begin
			
				marcarDireita  <= posicaoAtualnoEixoX + distanciaDireita - 1;
				marcarEsquerda <= posicaoAtualnoEixoX - distanciaEsquerda + 1;
			
				stage <= marcarHorizontal;
				
				malha[posicaoAtualnoEixoX + distanciaDireita][posicaoAtualnoEixoX] <= 2'b10;
				malha[posicaoAtualnoEixoX + distanciaEsquerda][posicaoAtualnoEixoX] <= 2'b10;
			
				// posições atuais da célula a ser marcada, começa na célula destino e vai até a posição atual do carrinho
				posicaoDireita <= posicaoAtualnoEixoX + distanciaDireita;
				posicaoEsquerda <= posicaoAtualnoEixoX - distanciaEsquerda;
			
			end
		end		
	end
end

always_ff @(posedge clock) begin

	case(stage) 
	
		IDLE: begin
		
			
		
		end

		marcarHorizontal: begin

			if(diretaAcabou && esquerdaAcabou) begin
			
				diretaAcabou <= 1'b0;
				esquerdaAcabou <= 1'b0;
				
				operacaoFinalizada <= 1'b1;
				
				stage <= IDLE;
			
			end
			else begin 
			
					stage <= marcarHorizontal;

				if(posicaoDireita != posicaoAtualnoEixoX) begin

					posicaoDireita  <= posicaoDireita - 1;
					
					malha[posicaoDireita][posicaoAtualnoEixoY] <= 2'b01;
					
				end
				else begin
				
					diretaAcabou <= 1'b1;
				
				end
				
				if(posicaoEsquerda != posicaoAtualnoEixoX) begin
				
					posicaoEsquerda <= posicaoEsquerda + 1;
					
					malha[posicaoEsquerda][posicaoAtualnoEixoX] <= 2'b01;
				
				end
				else begin
				
					esquerdaAcabou <= 1'b1;
				
				end
			
			end
		end			

		
		marcarVertical: begin
		
			if(diretaAcabou && esquerdaAcabou) begin
			
				diretaAcabou <= 1'b0;
				esquerdaAcabou <= 1'b0;
				
				operacaoFinalizada <= 1'b1;
				
				stage <= IDLE;
			
			end
			else begin 
			
					stage <= marcarHorizontal;

				if(posicaoDireita != posicaoAtualnoEixoY) begin

					posicaoDireita  <= posicaoDireita - 1;
					
					malha[posicaoDireita][posicaoAtualnoEixoY] <= 2'b01;
					
				end
				else begin
				
					diretaAcabou <= 1'b1;
				
				end
				
				if(posicaoEsquerda != posicaoAtualnoEixoX) begin
				
					posicaoEsquerda <= posicaoEsquerda + 1;
					
					malha[posicaoEsquerda][posicaoAtualnoEixoY] <= 2'b01;
				
				end
				else begin
				
					esquerdaAcabou <= 1'b1;
				
				end
			
			end
		
		end
		
	endcase

end

endmodule