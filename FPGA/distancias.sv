module distancias #(parameter int TamanhoMalha = 20, parameter int tamanhoDistancia = 8 /* em bits*/)(

	input clock,
	input reset,
	
	input [tamanhoDistancia - 1:0] posicaoAtualnoEixoX,
	input [tamanhoDistancia - 1:0] posicaoAtualnoEixoY,
	
	input novoDado,
	
	input [1:0] malha[TamanhoMalha*TamanhoMalha], //x,y - 00 - desconhecido, 01 - livre, 10 - ocupado,
	
	input [3:0] enable, // 1000 - direitaFrente, 0100 - esquerdaFrente, 0010 - direitaTras, 0001 - esquerdaTras
	
	output reg operacaoFinalizada,
	
	output reg [tamanhoDistancia - 1:0] destinoX,
	output reg [tamanhoDistancia - 1:0] destinoY

);

localparam IDLE = 2'b00,
		   incrementarRaio = 2'b01,
		   iniciarMedida = 2'b10,
		   esperarMedida = 2'b11;

reg [0:1] stage;

reg [TamanhoMalha - 1:0] contadorXdireitaFrente;
reg [TamanhoMalha - 1:0] contadorYdireitaFrente;
reg [TamanhoMalha - 1:0] contadorXdireitaTras;
reg [TamanhoMalha - 1:0] contadorYdireitaTras;

reg [TamanhoMalha - 1:0] contadorXesquerdaFrente;
reg [TamanhoMalha - 1:0] contadorYesquerdaFrente;
reg [TamanhoMalha - 1:0] contadorXesquerdaTras;
reg [TamanhoMalha - 1:0] contadorYesquerdaTras;

reg candidatoEncontradoFrenteDireita;
reg candidatoEncontradoFrenteEsquerda;
reg candidatoEncontradoTrasDireita;
reg candidatoEncontradoTrasEsquerda;

reg direitaFrenteAcabou;
reg esquerdaFrenteAcabou;
reg direitaTrasAcabou;
reg esquerdaTrasAcabou;

reg [tamanhoDistancia - 1:0] raio;

reg [tamanhoDistancia - 1:0] coordenadaCandidatoDireitaFrenteX; 
reg [tamanhoDistancia - 1:0] coordenadaCandidatoDireitaTrasX;
reg [tamanhoDistancia - 1:0] coordenadaCandidatoEsquerdaFrenteX;
reg [tamanhoDistancia - 1:0] coordenadaCandidatoEsquerdaTrasX;

reg [tamanhoDistancia - 1:0] coordenadaCandidatoDireitaFrenteY;
reg [tamanhoDistancia - 1:0] coordenadaCandidatoDireitaTrasY;
reg [tamanhoDistancia - 1:0] coordenadaCandidatoEsquerdaFrenteY;
reg [tamanhoDistancia - 1:0] coordenadaCandidatoEsquerdaTrasY;

reg [tamanhoDistancia - 1:0] candidatoAtualDireitaFrente;
reg [tamanhoDistancia - 1:0] candidatoAtualEsquerdaFrente;
reg [tamanhoDistancia - 1:0] candidatoAtualDireitaTras;
reg [tamanhoDistancia - 1:0] candidatoAtualEsquerdaTras;

// 01 - x atingiu o limite, 10 - y atingiu o limite, quando ambos atingem o limite, aquele setor acabou
reg [1:0] limiteAtingidoDireitaFrente; 
reg [1:0] limiteAtingidoDireitaTras; 
reg [1:0] limiteAtingidoEsquerdaFrente; 
reg [1:0] limiteAtingidoEsquerdaTras; 

reg enableDireitaFrente;
reg enableDireitaTras;
reg enableEsquerdaFrente;
reg enableEsquerdaTras;

reg raioAtualizado;

always_ff @(posedge clock, posedge reset) begin

	if(reset) begin
	
		enableDireitaFrente <= 0;
		enableDireitaTras <= 0;
		enableEsquerdaFrente <= 0;
		enableEsquerdaTras <= 0;
		
		
		limiteAtingidoDireitaFrente <= 2'b00;	
		limiteAtingidoEsquerdaFrente <= 2'b00;	
		limiteAtingidoDireitaTras <= 2'b00;	
		limiteAtingidoEsquerdaTras <= 2'b00;	
	
		operacaoFinalizada <= 0;
			
		destinoX <= 0;
		destinoY <= 0;
		
		stage <= IDLE;

		contadorXdireitaFrente <= 0;
		contadorYdireitaFrente <= 0;
		contadorXdireitaTras <= 0;
		contadorYdireitaTras <= 0;

		contadorXesquerdaFrente <= 0;
		contadorYesquerdaFrente <= 0;
		contadorXesquerdaTras <= 0;
		contadorYesquerdaTras <= 0;

		candidatoEncontradoFrenteDireita <= 0;
		candidatoEncontradoFrenteEsquerda <= 0;
		candidatoEncontradoTrasDireita <= 0;
		candidatoEncontradoTrasEsquerda <= 0;
		
		direitaFrenteAcabou <= 1;
		esquerdaFrenteAcabou <= 1;
		direitaTrasAcabou <= 1;
		esquerdaTrasAcabou <= 1;

		raio <= 0;

		coordenadaCandidatoDireitaFrenteX <= 0;
		coordenadaCandidatoEsquerdaFrenteX <= 0;
		coordenadaCandidatoDireitaTrasX <= 0;
		coordenadaCandidatoEsquerdaTrasX <= 0;
		
		coordenadaCandidatoDireitaFrenteY <= 0;
		coordenadaCandidatoEsquerdaFrenteY <= 0;
		coordenadaCandidatoDireitaTrasY <= 0;
		coordenadaCandidatoEsquerdaTrasY <= 0;

		//atribui valor máximo
		candidatoAtualDireitaFrente <= -1; 
		candidatoAtualDireitaTras <= -1; 
		candidatoAtualEsquerdaFrente <= -1; 
		candidatoAtualEsquerdaTras <= -1; 
	
		raioAtualizado <= 0;
	
	end
	else begin
	
		case(stage) 
		
			IDLE: begin
	
				if(novoDado) begin
				
					stage <= iniciarMedida;
				
				end
				else begin
				
					stage <= IDLE;
				
				end
			end
			
			iniciarMedida: begin
			
				candidatoEncontradoFrenteDireita <= 0;
				direitaFrenteAcabou <= !enable[3];
				enableDireitaFrente <= enable[3];
				
				candidatoEncontradoTrasDireita <= 0;
				direitaTrasAcabou <= !enable[1];
				enableDireitaTras <= enable[1];
				
				candidatoEncontradoFrenteEsquerda <= 0;
				esquerdaFrenteAcabou <= !enable[2];
				enableEsquerdaFrente <= enable[2];
				
				candidatoEncontradoTrasEsquerda <= 0;
				esquerdaTrasAcabou <= !enable[0];
				enableEsquerdaTras <= enable[0];
				
				limiteAtingidoDireitaFrente[1] <= !enable[3];	
				limiteAtingidoDireitaFrente[0] <= !enable[3];	
				
				limiteAtingidoEsquerdaFrente[1] <= !enable[2];	
				limiteAtingidoEsquerdaFrente[0] <= !enable[2];	
				
			
			    limiteAtingidoDireitaTras[1] <= !enable[1];	
				limiteAtingidoDireitaTras[0] <= !enable[1];	
				
				limiteAtingidoEsquerdaTras[1] <= !enable[0];	
				limiteAtingidoEsquerdaTras[0] <= !enable[0];	
				
				
				raio <= 1;
				
				//setar valores iniciais
				stage <= esperarMedida;
			
			end
			
			esperarMedida: begin
			
				raioAtualizado <= 0;
			
				if(direitaFrenteAcabou && direitaTrasAcabou && esquerdaFrenteAcabou && esquerdaTrasAcabou) begin
				
					if(candidatoEncontradoFrenteDireita || candidatoEncontradoTrasDireita || candidatoEncontradoFrenteEsquerda || candidatoEncontradoTrasEsquerda) begin
				
						enableDireitaFrente <= 0;
						enableDireitaTras <= 0;
						enableEsquerdaFrente <= 0;
						enableEsquerdaTras <= 0;	
					
					end
				
					stage <= incrementarRaio;
				
				end
				else begin
				
					stage <= esperarMedida;
				
				end
			
			end
			
			incrementarRaio: begin 
				//se algum lado acho candidato faz a conta do menor e acaba
				/*
				$display("---- CONDIÇÃO VERIFICADA ----");
				$display("candidatoEncontradoFrenteDireita=%b  limiteAtingidoDireitaFrente=%b",
						 candidatoEncontradoFrenteDireita, limiteAtingidoDireitaFrente);
				$display("candidatoEncontradoTrasDireita=%b  limiteAtingidoDireitaTras=%b",
						 candidatoEncontradoTrasDireita, limiteAtingidoDireitaTras);
				$display("candidatoEncontradoFrenteEsquerda=%b  limiteAtingidoEsquerdaFrente=%b",
						 candidatoEncontradoFrenteEsquerda, limiteAtingidoEsquerdaFrente);
				$display("candidatoEncontradoTrasEsquerda=%b  limiteAtingidoEsquerdaTras=%b",
						 candidatoEncontradoTrasEsquerda, limiteAtingidoEsquerdaTras);
				$display("---------------------------------\n");
				*/

				
				if((candidatoEncontradoFrenteDireita || limiteAtingidoDireitaFrente == 2'b11) && (candidatoEncontradoTrasDireita || limiteAtingidoDireitaTras == 2'b11) && (candidatoEncontradoFrenteEsquerda || limiteAtingidoEsquerdaFrente == 2'b11) && (candidatoEncontradoTrasEsquerda || limiteAtingidoEsquerdaTras == 2'b11)) begin
			
					stage <= IDLE;
					raio <= 1;
					//ja atribuir sinla de acabou e mandar par novo, além de resetar candidatos
					
					operacaoFinalizada <= 1;
					
					//ganhador
					
					$display("candidatoAtualDireitaFrente: %d", candidatoAtualDireitaFrente);
					$display("candidatoAtualDireitaTras: %d", candidatoAtualDireitaTras);
					$display("candidatoAtualEsquerdaFrente: %d", candidatoAtualEsquerdaFrente);
					$display("candidatoAtualEsquerdaTras: %d", candidatoAtualEsquerdaTras);
					
					//direitaFrente
					if(($signed(candidatoAtualDireitaFrente) >= $signed(candidatoAtualDireitaTras)) && ($signed(candidatoAtualDireitaFrente) >= $signed(candidatoAtualEsquerdaFrente)) && ($signed(candidatoAtualDireitaFrente) >= $signed(candidatoAtualEsquerdaTras))) begin
						
						destinoX <= coordenadaCandidatoDireitaFrenteX;
						destinoY <= coordenadaCandidatoDireitaFrenteY;
						
						$display("Direita Frente venceu");
						
					end
					else begin
					
						//direitaTras
						if(($signed(candidatoAtualDireitaFrente) >= $signed(candidatoAtualDireitaTras)) && ($signed(candidatoAtualDireitaFrente) >= $signed(candidatoAtualEsquerdaFrente)) && ($signed(candidatoAtualDireitaFrente) >= $signed(candidatoAtualEsquerdaTras))) begin
						
							destinoX <= coordenadaCandidatoDireitaTrasX;
							destinoY <= coordenadaCandidatoDireitaTrasY;
							
							$display("Direita Tras venceu");
					
						end
						else begin
							
							//esquerdaFrente
							if(($signed(candidatoAtualEsquerdaFrente) >= $signed(candidatoAtualDireitaFrente)) && ($signed(candidatoAtualEsquerdaFrente) >= $signed(candidatoAtualDireitaTras)) && ($signed(candidatoAtualEsquerdaFrente) >= $signed(candidatoAtualEsquerdaTras))) begin
							
								destinoX <= coordenadaCandidatoEsquerdaFrenteX;
								destinoY <= coordenadaCandidatoEsquerdaFrenteY;
							
								$display("Esquerda Frente venceu");
							
							end
							//esquerdaTras
							else begin
							
								destinoX <= coordenadaCandidatoEsquerdaTrasX;
								destinoY <= coordenadaCandidatoEsquerdaTrasY;
								
								$display("Esquerda Tras venceu");
							
							end
						end
					end
				end
				else begin
				
					stage <= esperarMedida;
					raio <= raio + 1;
					
					raioAtualizado <= 1;
					
				end
				
			end
		endcase
	end
end

distanciaDireitaFrente #(
    .TamanhoMalha(TamanhoMalha),
    .tamanhoDistancia(tamanhoDistancia)
)distanciaDireitaFrente (

	.clock(clock),
	.reset(reset),
	.enable(enableDireitaFrente),
	
	.raio(raio),
	
	.posicaoAtualnoEixoX(posicaoAtualnoEixoX),
	.posicaoAtualnoEixoY(posicaoAtualnoEixoY),
	
	.malha(malha),
	
	.operacaoFinalizada(candidatoEncontradoFrenteDireita),
	.acabouCalculoLocal(direitaFrenteAcabou),
	
	.candidatoAtual(candidatoAtualDireitaFrente),
	.coordenadaCandidatoX(coordenadaCandidatoDireitaFrenteX),
	.coordenadaCandidatoY(coordenadaCandidatoDireitaFrenteY),
	
	.raioAtualizado(raioAtualizado)

);

distanciaDireitaTras #(
    .TamanhoMalha(TamanhoMalha),
    .tamanhoDistancia(tamanhoDistancia)
)distanciaDireitaTras (

	.clock(clock),
	.reset(reset),
	.enable(enableDireitaTras),
	
	.raio(raio),
	
	.posicaoAtualnoEixoX(posicaoAtualnoEixoX),
	.posicaoAtualnoEixoY(posicaoAtualnoEixoY),
	
	.malha(malha),
	
	.operacaoFinalizada(candidatoEncontradoTrasDireita),
	.acabouCalculoLocal(direitaTrasAcabou),
	
	.candidatoAtual(candidatoAtualDireitaTras),
	.coordenadaCandidatoX(coordenadaCandidatoDireitaTrasX),
	.coordenadaCandidatoY(coordenadaCandidatoDireitaTrasY),
	
	.raioAtualizado(raioAtualizado)

);

distanciaEsquerdaFrente #(
    .TamanhoMalha(TamanhoMalha),
    .tamanhoDistancia(tamanhoDistancia)
)distanciaEsquerdaFrente (

	.clock(clock),
	.reset(reset),
	.enable(enableEsquerdaFrente),
	
	.raio(raio),
	
	.posicaoAtualnoEixoX(posicaoAtualnoEixoX),
	.posicaoAtualnoEixoY(posicaoAtualnoEixoY),
	
	.malha(malha),
	
	.operacaoFinalizada(candidatoEncontradoFrenteEsquerda),
	.acabouCalculoLocal(esquerdaFrenteAcabou),
		
	.candidatoAtual(candidatoAtualEsquerdaFrente),
	.coordenadaCandidatoX(coordenadaCandidatoEsquerdaFrenteX),
	.coordenadaCandidatoY(coordenadaCandidatoEsquerdaFrenteY),
	
	.raioAtualizado(raioAtualizado)

);

distanciaEsquerdaTras #(
    .TamanhoMalha(TamanhoMalha),
    .tamanhoDistancia(tamanhoDistancia)
)distanciaEsquerdaTras (

	.clock(clock),
	.reset(reset),
	.enable(enableEsquerdaTras),
	
	.raio(raio),
	
	.posicaoAtualnoEixoX(posicaoAtualnoEixoX),
	.posicaoAtualnoEixoY(posicaoAtualnoEixoY),
	
	.malha(malha),
	
	.operacaoFinalizada(candidatoEncontradoTrasEsquerda),
	.acabouCalculoLocal(esquerdaTrasAcabou),
	
	.candidatoAtual(candidatoAtualEsquerdaTras),
	.coordenadaCandidatoX(coordenadaCandidatoEsquerdaTrasX),
	.coordenadaCandidatoY(coordenadaCandidatoEsquerdaTrasY),
	
	.raioAtualizado(raioAtualizado)

);
endmodule