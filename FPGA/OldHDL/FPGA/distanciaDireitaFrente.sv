module distanciaDireitaFrente #(parameter int TamanhoMalha = 20, parameter int tamanhoDistancia = 8 /* em bits*/)(

	input clock,
	input reset,
	input enable,
	
	input [tamanhoDistancia - 1:0] raio,
	
	input [tamanhoDistancia - 1:0] posicaoAtualnoEixoX,
	input [tamanhoDistancia - 1:0] posicaoAtualnoEixoY,
	
	input [1:0] malha[TamanhoMalha*TamanhoMalha],
	
	input raioAtualizado,
	
	output reg operacaoFinalizada,
	
	output reg [tamanhoDistancia - 1:0] candidatoAtual,
	output reg [tamanhoDistancia - 1:0] coordenadaCandidatoX,
	output reg [tamanhoDistancia - 1:0] coordenadaCandidatoY,
	
	output reg acabouCalculoLocal
	
);

reg candidatoEncontrado;

reg [tamanhoDistancia - 1:0] contadorX;
reg [tamanhoDistancia - 1:0] contadorY;

reg [1:0] limiteAtingido;

reg [1:0] stage;

localparam IDLE = 2'b00,
		   Raio = 2'b01,
		   Calculo = 2'b10,
		   WAIT    = 2'b11;

//quadradinho analizado no momento
wire [tamanhoDistancia - 1:0] distancia;
assign distancia = (contadorX + contadorY) - (posicaoAtualnoEixoX + posicaoAtualnoEixoY);

always_ff @(posedge clock, posedge reset) begin

	if(reset) begin
	
		stage <= IDLE;
		
		acabouCalculoLocal <= 0;
		candidatoEncontrado <= 0;

		contadorX <= 0;
		contadorY <= 0;

		limiteAtingido <= 2'b11;
		
		operacaoFinalizada <= 0;
	
		candidatoAtual <= 0;
		coordenadaCandidatoX <= 0;
		coordenadaCandidatoY <= 0;
	
	end
	else begin
		
		case (stage) 
		
			IDLE: begin
			
				if(enable) begin
					// 
					if(posicaoAtualnoEixoX + 1 == TamanhoMalha) begin
				
						limiteAtingido[0] <= 1'b1;
						
					end
					else begin
						
						contadorX <= posicaoAtualnoEixoX + 1;
						limiteAtingido[0] <= 1'b0;
					
					end
					
					contadorY <= posicaoAtualnoEixoY;
					
					stage <= Calculo;
					
					candidatoAtual <= -1;
					
					limiteAtingido[1] <= 1'b0;
					
				end
				else begin
				
					stage <= IDLE;
					operacaoFinalizada <= 0;
					candidatoEncontrado <= 0;

					
				end

			end
		
			Calculo: begin
				
				//só começa se não tiver acabado, fazer para todos
				if(!acabouCalculoLocal && enable) begin 
					// local válido
					$display("DireitaFrente - Posicao X: %d, Posicao Y: %d, Valor na Malha: %d", contadorX, contadorY, malha[contadorX + contadorY*TamanhoMalha]);
					
					if(malha[contadorX + contadorY*TamanhoMalha] == 3) begin 
					
						candidatoEncontrado <= 1;
						
							//valor ja escolhido	        novoCandidato
						if(candidatoAtual > distancia) begin
						
							//novo menor caminho guardado
							candidatoAtual <= distancia;
							coordenadaCandidatoX <= contadorX;
							coordenadaCandidatoY <= contadorY;
						
						end
					
					end

					if(contadorX == raio + posicaoAtualnoEixoX || limiteAtingido[0] == 1'b1) begin
					
						if(contadorY == raio + posicaoAtualnoEixoY || limiteAtingido[1] == 1'b1) begin
						
							//acabou busca nesse raio
							acabouCalculoLocal <= 1;
							
							stage <= Raio;
						
						end
						else begin
						
							if(contadorY == TamanhoMalha - 1) begin
						
								limiteAtingido[1] <= 1'b1;
								
							end
							else begin
							
								contadorY <= contadorY + 1;
							
							end
							//rever
							if(contadorY == raio + posicaoAtualnoEixoY - 1) begin 
							
								if(contadorX == TamanhoMalha - 1) begin
						
									limiteAtingido[0] <= 1'b1;
								
								end
								else begin
							
									contadorX <= posicaoAtualnoEixoX;
								
								end
							end
						
						end
					
					end
					else begin
					
						if(contadorX == TamanhoMalha - 1) begin
						
								limiteAtingido[0] <= 1'b1;
								
						end
						else begin
						
							contadorX <= contadorX + 1;
						
						end
					end
				end
			end
			
			Raio: begin
			
				//direita Frente
				if(posicaoAtualnoEixoX + raio == TamanhoMalha - 1) begin

					limiteAtingido[0] <= 1'b1;
				
				end
				else begin
					
					contadorX <= posicaoAtualnoEixoX + raio + 1;
			
				end
				
				contadorY <= posicaoAtualnoEixoY;
				
				if(limiteAtingido == 2'b11 || candidatoEncontrado) begin
				
					operacaoFinalizada <= 1;
					stage <= WAIT;
					acabouCalculoLocal <= 1;
					
				end
				else begin
				
					stage <= WAIT;
				
				end
			
			end
			
			//espera atualizar o raio
			WAIT: begin
			
				
			
				if(raioAtualizado) begin 
				
					
					if(limiteAtingido == 2'b11 || candidatoEncontrado) begin
					
						stage <= IDLE;
						
					end
					else begin
					
						stage <= Calculo;
						acabouCalculoLocal <= 0;
					
					end
				end
				else begin
				
					stage <= WAIT;
				
				end
			
			end
			
		endcase
	end
end

endmodule