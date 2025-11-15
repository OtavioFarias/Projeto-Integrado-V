`timescale 1ns/1ps

module tb;

// Entradas do DUT

parameter int TamanhoMalha = 15;
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
	
	.ready(ready)

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
    /*
    valor linear X | valor linear Y	 | Valor da Célula | Pai | Custo Acumulado | Custo Estimado | open
	Para Malha 15:
	4 | 4 | 2 | 2 | 8 | 8 | 1
	0001 | 0001 | 01 | 11 | 00000000 | 00000000 | 0
	*/
	
	//address = x + y * TAMANHO_MALHA
    addressFromNIOS = 16;
    dataFromNIOS = 29'b0001_0001_01_11_01000000_00000000_0;
    writeFromNIOS = 1;

	@(posedge clock);
	
	addressFromNIOS = 14;
    dataFromNIOS = 29'b1110_0000_01_11_01000000_00000000_0;
    writeFromNIOS = 1;

	@(posedge clock);
	
	addressFromNIOS = 0;
    dataFromNIOS = 29'b0000_0000_01_11_01000000_00000000_0;
    writeFromNIOS = 1;

	@(posedge clock);
	
	addressFromNIOS = 30;
    dataFromNIOS = 29'b0000_0010_01_11_01000000_00000000_0;
    writeFromNIOS = 1;

	@(posedge clock);

    // Teste 1
    writeFromNIOS = 0;
    
    @(posedge clock);
    
    addressFromNIOS = 15;
    dataFromNIOS = 29'b0000_0001_01_00_00000000_00000000_0;
    requestHW = 1;
    
    @(posedge clock);
    
    wait(ready == 0)
    
    requestHW = 0;
    
    @(posedge clock);
    
	/*
    // Teste 2
    #40;
    address = 32'h0000_0008;
    dataFromNIOS = 32'hDEADBEEF;
    direcaoFromNIOS = 2'b10;
    requestHW = 1;

    #20;
    requestHW = 0;
	*/
    // Finaliza simulação
    #200;
    $finish;
end

endmodule
