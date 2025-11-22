`timescale 1ns/1ps

module tb;

// Entradas do DUT

parameter int TamanhoMalha = 10;
parameter int tamanhoDados = 6 * $clog2(TamanhoMalha) + 5;
parameter addressTam = 2 * $clog2(TamanhoMalha);

reg clock;
reg reset;
reg requestHW;

reg [addressTam - 1:0] address;
reg [tamanhoDados - 1:0] dataFromNIOS;

reg [addressTam - 1 : 0] addressFromNIOS;
reg readFromNIOS;
reg writeFromNIOS;
reg readFIFO;

// Saídas do DUT (adicione aqui caso seu módulo tenha)
wire alguma_saida;

// Instância do módulo em teste (renomeie "meu_modulo" para o nome real)
Top 
#(.TamanhoMalha(TamanhoMalha), .tamanhoDados(tamanhoDados), .addressTam(addressTam))
DUT (
    .clock(clock),
    .reset(reset),
    
    .requestFromNIOS(requestHW),
    .addressFromNIOS(addressFromNIOS),    
    .dataFromNIOS(dataFromNIOS),
    
	.readFromNIOS(readFromNIOS),
	.writeFromNIOS(writeFromNIOS),
	.readFIFO(readFIFO),
	
	.ready(ready),
	.readyFIFO(readyFIFO)

);

// Geração de clock
always #5 clock = ~clock;

// Sequência de testes
initial begin
    // Inicialização
    clock = 0;
    reset = 1;
    requestHW = 0;
    address = 0;
    dataFromNIOS = 0;
    addressFromNIOS = 0;
	readFromNIOS = 0;
	writeFromNIOS = 0;
	readFIFO = 0;

    // Libera reset
    @(posedge clock);
    
    reset = 0;

	// Carregando Valor na Malha para teste inicial
	
	@(posedge clock);

	addressFromNIOS = 70;
	dataFromNIOS = 29'b0000_0111_11_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 71;
	dataFromNIOS = 29'b0001_0111_11_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 72;
	dataFromNIOS = 29'b0010_0111_11_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 73;
	dataFromNIOS = 29'b0011_0111_11_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 74;
	dataFromNIOS = 29'b0100_0111_11_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 80;
	dataFromNIOS = 29'b0000_1000_01_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 81;
	dataFromNIOS = 29'b0001_1000_01_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 82;
	dataFromNIOS = 29'b0010_1000_01_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 83;
	dataFromNIOS = 29'b0011_1000_01_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 84;
	dataFromNIOS = 29'b0100_1000_01_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 85;
	dataFromNIOS = 29'b0101_1000_11_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 90;
	dataFromNIOS = 29'b0000_1001_11_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 91;
	dataFromNIOS = 29'b0001_1001_11_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 92;
	dataFromNIOS = 29'b0010_1001_11_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 93;
	dataFromNIOS = 29'b0011_1001_11_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	addressFromNIOS = 94;
	dataFromNIOS = 29'b0100_1001_11_0000000000000000000;
	writeFromNIOS = 1;
	
	@(posedge clock);

	writeFromNIOS = 0;
    
    @(posedge clock);
    /*
    valor linear X | valor linear Y	 | Valor da Célula | Pai | Custo Acumulado | Custo Estimado | open
	Para Malha 15:
	4 | 4 | 2 | 2 | 8 | 8 | 1
	0001 | 0001 | 01 | 11 | 00000000 | 00000000 | 0
	*/
	//Posição X: 1 	Posição Y: 8 	Distância Direita: 3	 Distância Esquerda: 2
	//address = x + y * TAMANHO_MALHA
    //addressFromNIOS = 81;
    //dataFromNIOS = 29'b00011000010000000000000000011;
    
	addressFromNIOS = 81;
	dataFromNIOS = 29'b0001_1000_01_00_00000000_00000001_1;
    requestHW = 1;
    
    @(posedge clock);
    
    wait(ready == 0)
    
    requestHW = 0;
    
    @(posedge clock);
    
    wait(ready == 1);
    
    //Leitura da Fila
    
    @(posedge clock)
    
	while(!DUT.emptyFIFO) begin
    
    		@(posedge clock) 
    
		if(readyFIFO) begin
		
			readFIFO <= 1;
		
		end
		else begin
		
			readFIFO <= 0;
		
		end
    
    end
    
    #200;
    $finish;
    
end

endmodule
