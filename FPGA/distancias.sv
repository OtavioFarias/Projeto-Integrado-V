

// arrumar o bit que indica fronteira


module distancias #(parameter int TamanhoMalha = 20, parameter int tamanhoDistancia = 8 /* em bits*/)(

	input clock,
	input reset,
	
	input [tamanhoDistancia - 1:0] posicaoAtualnoEixoX,
	input [tamanhoDistancia - 1:0] posicaoAtualnoEixoY,
	
	input novoDado,
	
	input [1:0] malha[TamanhoMalha*TamanhoMalha], //x,y - 00 - desconhecido, 01 - livre, 10 - ocupado,
	
	input [3:0] enable, // 0001 - direita,Frente, 0010 - esquerdaFrente, 0100 - direitaTras, 1000 - esquerdaTras
	
	output reg operacaoFinalizada,
	
	output reg [tamanhoDistancia - 1:0] destinoX,
	output reg [tamanhoDistancia - 1:0] destinoY

);

localparam IDLE = 2'b00,
		   incrementarRaio = 2'b01,
		   iniciarMedida = 2'b10,
		   esperarMedida = 2'b11;

reg [0:1] stage;

reg direitaFrentePronta;
reg direitaTrasPronta;
reg esquerdaFrentePronta;
reg esquerdaTrasPronta;

reg [TamanhoMalha - 1:0] contadorXdireitaFrente;
reg [TamanhoMalha - 1:0] contadorYdireitaFrente;
reg [TamanhoMalha - 1:0] contadorXdireitaTras;
reg [TamanhoMalha - 1:0] contadorYdireitaTras;

reg [TamanhoMalha - 1:0] contadorXesquerdaFrente;
reg [TamanhoMalha - 1:0] contadorYesquerdaFrente;
reg [TamanhoMalha - 1:0] contadorXesquerdaTras;
reg [TamanhoMalha - 1:0] contadorYesquerdaTras;

reg candidatoEncontradoFrenteDireita;
reg direitaAcabou;

reg [tamanhoDistancia - 1:0] raio;

reg [tamanhoDistancia - 1:0] coordenadaCandidatoDireitaFrenteX;
reg [tamanhoDistancia - 1:0] coordenadaCandidatoDireitaFrenteY;

reg [tamanhoDistancia - 1:0] candidatoAtualDireitaFrente;

reg [1:0] limiteAtingidoDireitaFrente; // 01 - x atingiu o limite, 10 - y atingiu o limite, quando ambos atingem o limite, aquele setor acabou

always_ff @(posedge clock, posedge reset) begin

	if(reset) begin
	
		limiteAtingidoDireitaFrente <= 2'b00;	
	
		operacaoFinalizada <= 0;
			
		destinoX <= 0;
		destinoY <= 0;
		
		stage <= IDLE;
	
		direitaFrentePronta <= 0;
		direitaTrasPronta <= 0;
		esquerdaFrentePronta <= 0;
		esquerdaTrasPronta <= 0;

		contadorXdireitaFrente <= 0;
		contadorYdireitaFrente <= 0;
		contadorXdireitaTras <= 0;
		contadorYdireitaTras <= 0;

		contadorXesquerdaFrente <= 0;
		contadorYesquerdaFrente <= 0;
		contadorXesquerdaTras <= 0;
		contadorYesquerdaTras <= 0;

		candidatoEncontradoFrenteDireita <= 0;
		direitaAcabou <= 1;

		raio <= 0;

		coordenadaCandidatoDireitaFrenteX <= 0;
		coordenadaCandidatoDireitaFrenteY <= 0;

		candidatoAtualDireitaFrente <= -1; //atribui valor máximo

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
				direitaAcabou <= 0;
				
				raio <= 1;
				
				if(posicaoAtualnoEixoX + 1 == TamanhoMalha) begin
			
					limiteAtingidoDireitaFrente[0] <= 1'b1;
					
				end
				else begin
					
					contadorXdireitaFrente <= posicaoAtualnoEixoX + 1;
				
				end
				
				contadorYdireitaFrente <= posicaoAtualnoEixoY;
				
				stage <= esperarMedida;
			
			end
			
			esperarMedida: begin
			
				//fazer isso para todos
				if(direitaAcabou) begin
				
					stage <= incrementarRaio;
				
				end
				else begin
				
					stage <= esperarMedida;
				
				end
			
			end
			
			incrementarRaio: begin 
				//se algum lado acho candidato faz a conta do menor e acaba
				
				if(candidatoEncontradoFrenteDireita || limiteAtingidoDireitaFrente == 2'b11) begin
			
					stage <= IDLE;
					raio <= 1;
					//ja atribuir sinla de acabou e mandar par novo, além de resetar candidatos
					
					operacaoFinalizada <= 1;
					
					//ganhador
					destinoX <= coordenadaCandidatoDireitaFrenteX;
					destinoY <= coordenadaCandidatoDireitaFrenteY;
			
				end
				else begin
				
					stage <= esperarMedida;
					raio <= raio + 1;
					
					if(posicaoAtualnoEixoX + raio == TamanhoMalha - 1) begin
			
						limiteAtingidoDireitaFrente[0] <= 1'b1;
					
					end
					else begin
						
						contadorXdireitaFrente <= posicaoAtualnoEixoX + raio + 1;
				
					end
					
					contadorYdireitaFrente <= posicaoAtualnoEixoY;
					direitaAcabou <= 0;
					
				end
				
			end
		endcase
	end
end

//quadradinho analizado no momento
wire [tamanhoDistancia - 1:0] distanciaDireitaFrente;
assign distanciaDireitaFrente =  (contadorXdireitaFrente + contadorYdireitaFrente) - (posicaoAtualnoEixoX + posicaoAtualnoEixoY);

always_ff @(posedge clock) begin
	
	//só começa se não tiver acabado, fazer para todos
	if(!direitaAcabou && enable[0] == 1'b1) begin 
		// local válido
		
		if(malha[contadorXdireitaFrente + contadorYdireitaFrente*TamanhoMalha] == 3) begin 
		
			candidatoEncontradoFrenteDireita <= 1;
		
				//valor ja escolhido	        novoCandidato
			if(candidatoAtualDireitaFrente > distanciaDireitaFrente) begin
			
				//novo menor caminho guardado
				candidatoAtualDireitaFrente <= distanciaDireitaFrente;
				coordenadaCandidatoDireitaFrenteX <= contadorXdireitaFrente;
				coordenadaCandidatoDireitaFrenteY <= contadorYdireitaFrente;
			
			end
		
		end

		if(contadorXdireitaFrente == raio + posicaoAtualnoEixoX || limiteAtingidoDireitaFrente[0] == 1'b1) begin
		
			if(contadorYdireitaFrente == raio + posicaoAtualnoEixoY || limiteAtingidoDireitaFrente[1] == 1'b1) begin
			
				//acabou busca nesse raio
				direitaAcabou <= 1;
			
			end
			else begin
			
				if(contadorYdireitaFrente == TamanhoMalha - 1) begin
			
					limiteAtingidoDireitaFrente[1] <= 1'b1;
					
				end
				else begin
				
					contadorYdireitaFrente <= contadorYdireitaFrente + 1;
				
				end
				if(contadorYdireitaFrente == raio + posicaoAtualnoEixoX - 1) begin 
				
					if(contadorXdireitaFrente == TamanhoMalha - 1) begin
			
						limiteAtingidoDireitaFrente[0] <= 1'b1;
					
					end
					else begin
				
						contadorXdireitaFrente <= posicaoAtualnoEixoX;
					
					end
				end
			
			end
		
		end
		else begin
		
			if(contadorXdireitaFrente == TamanhoMalha - 1) begin
			
					limiteAtingidoDireitaFrente[0] <= 1'b1;
					
			end
			else begin
			
				contadorXdireitaFrente <= contadorXdireitaFrente + 1;
			
			end
		end
	end
end

endmodule