`timescale 1ns/1ps

module tb;

// Entradas do DUT
reg clock;
reg reset;
reg requestHW;

reg [31:0] address;
reg [31:0] dataFromNIOS;
reg [1:0]  direcaoFromNIOS;

// Saídas do DUT (adicione aqui caso seu módulo tenha)
wire alguma_saida;

// Instância do módulo em teste (renomeie "meu_modulo" para o nome real)
Top DUT (
    .clock(clock),
    .reset(reset),
    .requestHW(requestHW),
    
    .address(address),
    .dataFromNIOS(dataFromNIOS),
    .direcaoFromNIOS(direcaoFromNIOS)

    // .saida(alguma_saida)   // se houver
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
    direcaoFromNIOS = 0;

    // Libera reset
    #20;
    reset = 0;

    // Teste 1
    #20;
    address = 32'h0000_0004;
    dataFromNIOS = 32'hABCD1234;
    direcaoFromNIOS = 2'b01;
    requestHW = 1;

    #20;
    requestHW = 0;

    // Teste 2
    #40;
    address = 32'h0000_0008;
    dataFromNIOS = 32'hDEADBEEF;
    direcaoFromNIOS = 2'b10;
    requestHW = 1;

    #20;
    requestHW = 0;

    // Finaliza simulação
    #200;
    $finish;
end

endmodule
