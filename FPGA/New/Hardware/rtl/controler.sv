module controler #(
parameter int TamanhoMalha = 15, 
parameter int tamanhoDados = 6 * $clog2(TamanhoMalha) + 5,
parameter addressTam = 2 * $clog2(TamanhoMalha)
)(

	input clock,
	input reset,

	input requestFromNIOS,
	output reg ready,

	input [addressTam - 1 : 0] addressFromNIOS,
	input [tamanhoDados - 1:0] dataFromNIOS,
	input write,

	output reg requestToHW,
	output reg [addressTam - 1 : 0] addressToHW,
	output reg [tamanhoDados - 1:0] dataToHW,
	output reg [1:0] direcaoToHW
	
);

reg state;

parameter IDLE = 1'b0,
		  WAIT_HW = 1'b1;
		  
always_ff @(posedge clock, posedge reset) begin

	if(reset) begin
	
		state <= IDLE;
		ready <= 1;
		requestToHW <= 0;
		addressToHW <= 0;
		dataToHW <= 0;
		direcaoToHW <= 0;
	
	end
	else begin
	
		case(state) 
		
			IDLE: begin
			
				if(requestFromNIOS) begin
				
					ready <= 0;
					state <= WAIT_HW;
					requestToHW <= 1;
					addressToHW <= addressFromNIOS;
					dataToHW <= dataFromNIOS;
				
				end
				else begin
				
					ready <= 1;
					requestToHW <= 0;
					state <= IDLE;
				
				end
			
			end
			
			WAIT_HW: begin
			
				if(direcaoToHW == 2'b11) begin
				
					requestToHW <= 0;
					ready <= 1;
					state <= IDLE;
					direcaoToHW <= 0;
				
			
				
				end
				else begin
				
					direcaoToHW <= direcaoToHW + 1;
					ready <= 0;
					requestToHW <= 1;
					state <= WAIT_HW;
				
				end
			
			end
		
		endcase
	
	end

end

endmodule
