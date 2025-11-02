module mapas #(parameter int TamanhoMalha = 20, parameter int tamanhoDistancia = 8 /* em bits*/)(

	input clock,
	input reset,
	
	input [tamanhoDistancia - 1:0] posicaoAtualnoEixoX,
	input [tamanhoDistancia - 1:0] posicaoAtualnoEixoY,
	input direcaoAtual, // 1 - horizontal, 0 - vertical
	
	input [tamanhoDistancia - 1:0] distanciaDireita,
	input [tamanhoDistancia - 1:0] distanciaEsquerda,
	
	input novoDado,
	
	
	output reg [1:0] malha[TamanhoMalha*TamanhoMalha], //x,y - 00 - desconhecido, 01 - livre, 10 - ocupado
	output reg operacaoFinalizada // valor 1 se operação finalizada e disponível para receber novas requisições

);

localparam marcarVertical = 2'b01,
		   marcarHorizontal = 2'b10,
		   IDLE = 2'b00;

reg [tamanhoDistancia - 1:0] marcarDireita;
reg [tamanhoDistancia - 1:0] marcarEsquerda;

reg [tamanhoDistancia - 1:0] posicaoDireita;
reg [tamanhoDistancia - 1:0] posicaoEsquerda;

reg direitaAcabou;
reg esquerdaAcabou;

reg [1:0] stage;

wire [tamanhoDistancia - 1:0] posicaoEsquerdaX;
wire [tamanhoDistancia - 1:0] posicaoEsquerdaY;

wire [tamanhoDistancia - 1:0] posicaoDireitaX;
wire [tamanhoDistancia - 1:0] posicaoDireitaY;

assign posicaoEsquerdaX = posicaoAtualnoEixoX - distanciaEsquerda;
assign posicaoEsquerdaY = posicaoAtualnoEixoY - distanciaEsquerda;
assign posicaoDireitaX = posicaoAtualnoEixoX + distanciaDireita;
assign posicaoDireitaY = posicaoAtualnoEixoY + distanciaDireita;

integer x,y;

always_ff @(posedge clock, posedge reset) begin

	if(reset) begin
	
		 for (x = 0; x < TamanhoMalha; x++) begin
		 	for (y = 0; y < TamanhoMalha; y++) begin
		 
		 		malha[x*TamanhoMalha+y] <= 2'b00;
		 	
		 	end
		 end
		 
		operacaoFinalizada <= 1'b1;
		stage <= IDLE;
		 
		marcarDireita <= 0;
		marcarEsquerda <= 0;
		
		direitaAcabou <= 1'b0;
		esquerdaAcabou <= 1'b0;
		 
		posicaoDireita <= 0;
		posicaoEsquerda <= 0;
		 
	end
	else begin
		
		case(stage) 
	
		IDLE: begin
			
			if(novoDado) begin //está errado, esse é horizontal
			
				operacaoFinalizada <= 1'b0;
				
				//marca posição atual
				malha[posicaoAtualnoEixoX+posicaoAtualnoEixoY*TamanhoMalha] <= 2'b01;
					
				// mandar dados para a unidade responsável por marcar
				// aqui os conceitos de direita e esquerda são em relação a posição atual do carrinho
				if(direcaoAtual) begin // horizontal

					marcarDireita  <=  posicaoDireitaX + 1;
					marcarEsquerda <= posicaoEsquerdaX - 1;
					
					// posições atuais da célula a ser marcada, começa na posição atual do carrinho e direciona para até a célula destino
					posicaoDireita <= posicaoAtualnoEixoX + 1;
					posicaoEsquerda <= posicaoAtualnoEixoX - 1;
					
					stage <= marcarHorizontal;
					
					malha[posicaoDireitaX+posicaoAtualnoEixoY*TamanhoMalha+1] <= 2'b10;
					malha[posicaoEsquerdaX+posicaoAtualnoEixoY*TamanhoMalha-1] <= 2'b10;
					
				
					if(malha[posicaoAtualnoEixoX+(posicaoAtualnoEixoY - 1)*TamanhoMalha] != 1) begin
					
						malha[posicaoAtualnoEixoX+(posicaoAtualnoEixoY - 1)*TamanhoMalha] <= 2'b11;
					
					end
					
					
					if(malha[posicaoAtualnoEixoX+(posicaoAtualnoEixoY + 1)*TamanhoMalha] != 1) begin
					
						malha[posicaoAtualnoEixoX+(posicaoAtualnoEixoY + 1)*TamanhoMalha] <= 2'b11;
					
					end
				
				end
				else begin // vertical
				
					marcarDireita  <= posicaoDireitaY + 1;
					marcarEsquerda <= posicaoEsquerdaY - 1;
				
					// posições atuais da célula a ser marcada, começa na célula destino e vai até a posição atual do carrinho
					posicaoDireita <= posicaoAtualnoEixoY + 1;
					posicaoEsquerda <= posicaoAtualnoEixoY - 1;
				
					stage <= marcarVertical;
					
					malha[posicaoAtualnoEixoX+(posicaoDireitaY+1)*TamanhoMalha] <= 2'b10;
					malha[posicaoAtualnoEixoX+(posicaoEsquerdaY-1)*TamanhoMalha] <= 2'b10;
					
					if(malha[posicaoAtualnoEixoX+posicaoAtualnoEixoY*TamanhoMalha + 1] != 1) begin
					
						malha[posicaoAtualnoEixoX+posicaoAtualnoEixoY*TamanhoMalha + 1] <= 2'b11;
					
					end
					
					
					if(malha[posicaoAtualnoEixoX+posicaoAtualnoEixoY*TamanhoMalha - 1] != 1) begin
					
						malha[posicaoAtualnoEixoX+posicaoAtualnoEixoY*TamanhoMalha - 1] <= 2'b11;
					
					end
				
				end
			end
			else begin
			
				stage <= IDLE;
			
			end
		end

		marcarHorizontal: begin

			if(direitaAcabou && esquerdaAcabou) begin
			
				direitaAcabou <= 1'b0;
				esquerdaAcabou <= 1'b0;
				
				operacaoFinalizada <= 1'b1;
				
				stage <= IDLE;
			
			end
			else begin 
			
				stage <= marcarHorizontal;

				if(posicaoDireita != marcarDireita) begin

					posicaoDireita  <= posicaoDireita + 1;
					
					malha[posicaoDireita+posicaoAtualnoEixoY*TamanhoMalha] <= 2'b01;
						
					if(malha[posicaoDireita+(posicaoAtualnoEixoY + 1)*TamanhoMalha] == 0) begin
					
						malha[posicaoDireita+(posicaoAtualnoEixoY + 1)*TamanhoMalha] <= 2'b11;
					
					end
					
					
					if(malha[posicaoDireita+(posicaoAtualnoEixoY - 1)*TamanhoMalha] == 0) begin
					
						malha[posicaoDireita+(posicaoAtualnoEixoY - 1)*TamanhoMalha] <= 2'b11;
					
					end
				
					
				end
				else begin
				
					direitaAcabou <= 1'b1;
				
				end
				
				if(posicaoEsquerda != marcarEsquerda) begin
				
					posicaoEsquerda <= posicaoEsquerda - 1;
					
					malha[posicaoEsquerda+posicaoAtualnoEixoY*TamanhoMalha] <= 2'b01;
						
					if(malha[posicaoEsquerda+(posicaoAtualnoEixoY + 1)*TamanhoMalha] == 0) begin
					
						malha[posicaoEsquerda+(posicaoAtualnoEixoY + 1)*TamanhoMalha] <= 2'b11;
					
					end
					
					
					if(malha[posicaoEsquerda+(posicaoAtualnoEixoY - 1)*TamanhoMalha] == 0) begin
					
						malha[posicaoEsquerda+(posicaoAtualnoEixoY - 1)*TamanhoMalha] <= 2'b11;
					
					end
				
				end
				else begin
				
					esquerdaAcabou <= 1'b1;
				
				end
			
			end
		end			
		
		marcarVertical: begin

			if(direitaAcabou && esquerdaAcabou) begin
			
				direitaAcabou <= 1'b0;
				esquerdaAcabou <= 1'b0;
				
				operacaoFinalizada <= 1'b1;
				
				stage <= IDLE;
			
			end
			else begin 
			
				stage <= marcarVertical;

				if(posicaoDireita != marcarDireita) begin

					posicaoDireita  <= posicaoDireita + 1;
					
					malha[posicaoDireita*TamanhoMalha+posicaoAtualnoEixoX] <= 2'b01;
					
					if(malha[posicaoDireita*TamanhoMalha+posicaoAtualnoEixoX + 1] != 1) begin
					
						malha[posicaoDireita*TamanhoMalha+posicaoAtualnoEixoX + 1] <= 2'b11;
					
					end
					
					
					if(malha[posicaoDireita*TamanhoMalha+posicaoAtualnoEixoX - 1] != 1) begin
					
						malha[posicaoDireita*TamanhoMalha+posicaoAtualnoEixoX - 1] <= 2'b11;
					
					end
					
				end
				else begin
				
					direitaAcabou <= 1'b1;
				
				end
				
				if(posicaoEsquerda != marcarEsquerda) begin
				
					posicaoEsquerda <= posicaoEsquerda - 1;
					
					malha[posicaoEsquerda*TamanhoMalha+posicaoAtualnoEixoX] <= 2'b01;
					
						
					if(malha[posicaoEsquerda*TamanhoMalha+posicaoAtualnoEixoX + 1] != 1) begin
					
						malha[posicaoEsquerda*TamanhoMalha+posicaoAtualnoEixoX + 1] <= 2'b11;
					
					end
					
					
					if(malha[posicaoEsquerda*TamanhoMalha+posicaoAtualnoEixoX - 1] != 1) begin
					
						malha[posicaoEsquerda*TamanhoMalha+posicaoAtualnoEixoX - 1] <= 2'b11;
					
					end
				
				end
				else begin
				
					esquerdaAcabou <= 1'b1;
				
				end
			
			end
		end			
		
		
	endcase

				
	end
end

endmodule