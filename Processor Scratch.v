Instruction
19  14|13 14|13 10|9 6|5 0|
opcode|Rout|Rin1|Rin2|other
opcode|Rout|Rin1|immediate
{
**
module Instruction_Fetch
**
input CLK, RESET,
input [INSTR_LENGTH: 0] Instruction_IN,
input [ADDR_LENGTH: 0] Program_Counter_IN,
input Request_Alt_Program_Counter_IN,
input [ADDR_LENGTH: 0] Alternative_Program_Counter_IN,
output reg [INSTR_LENGTH: 0] Instruction_OUT,
output reg [ADDR_LENGTH: 0] Program_Counter_OUT,
output reg [ADDR_LENGTH: 0] Next_Program_Counter_OUT,

assign Program_Counter_Init = ADDR_LENGTH'b0;
assign Increment_Amount = ADDR_LENGTH'b4;    // PC = PC + 4
assign Program_Counter = (Request_Alt_Program_Counter_IN == 1) ? (Alternative_Program_Counter_IN):(Next_Program_Counter_OUT);


always@(posedge CLK or negedge RESET) begin
	if (RESET) begin
		Instruction_OUT <= 0;
		Program_Counter_OUT <= 0;
		Next_Program_Counter_OUT <= Program_Counter_Init;
	end
	else if (CLK) begin
		Instruction_OUT <= Instruction_IN;
		Program_Counter_OUT <= Program_Counter
		Next_Program_Counter_OUT <= Program_Counter + Increment_Amount;
	end

end

endmodule}
{
module Instruction_Decode



endmodule
}
{
module Register_File

	input CLK,
	input RESET,
	input [ADDR_LENGTH:0] RegisterA,
	input [ADDR_LENGTH:0] RegisterB,
	input [ADDR_LENGTH:0] RegisterC,
	output [DATA_LENGTH:0] DataA,
	output [DATA_LENGTH:0] DataB,
	output [DATA_LENGTH:0] DataC,
	input [ADDR_LENGTH:0] WriteReg,
	input [DATA_LENGTH:0] WriteData,
	input Write
	
	
	reg [9:0] Reg [0:15];
	
	assign DataA = Reg[RegisterA];
	assign DataB = Reg[RegisterB];
	assign DataC = Reg[RegisterC];
	
	always @(posedge CLK or negedge RESET) begin
		if (!RESET) begin
			Reg[0] <= 0;
			Reg[1] <= 0;
			Reg[2] <= 0;
			Reg[3] <= 0;
			Reg[4] <= 0;
			Reg[5] <= 0;
			Reg[6] <= 0;
			Reg[7] <= 0;
			Reg[8] <= 0;
			Reg[9] <= 0;
			Reg[10] <= 0;
			Reg[11] <= 0;
			Reg[12] <= 0;
			Reg[13] <= 0;
			Reg[14] <= 0;
			Reg[15] <= 0;
		end
		else begin
			if (Write) begin
				if (WriteReg == 0) begin
					Reg[0] <= 0;
				end
				else begin
					Reg[WriteReg] <=WriteData;
				end
			end
		end
	end
	/**
	Reg[0] $zero
	Reg[1] $function return
	Reg[2] $function return
	Reg[3] $function arguments
	Reg[4] $function arguments
	Reg[5] $function arguments
	Reg[6] $temporary
	Reg[7] $temporary
	Reg[8] $temporary
	Reg[9] $temporary
	Reg[10] $temporary
	Reg[11] $temporary
	Reg[12] $global pointer
	Reg[13] $stack pointer
	Reg[14] $frame pointer
	Reg[15] $return address
	**/

endmodule
}

{
**
module ALU
**
input CLK,
input [DATA_LENGTH:0] Operand1,
input [DATA_LENGTH:0] Operand2,
input [INSTR_LENGTH:0] Instruction_IN,
output [DATA_LENGTH:0] ALU_Result,
output [XXX:0] Result_Register,
output [INSTR_LENGTH:0] Instruction_OUT,
/**
debug signals
**/
output Load_OUT,
output Store_OUT,
output Writeback_OUT,
output Overflow,
output Zero,
output Negative


assign opcode = Instruction_IN[19:14];

always @(posedge CLK) begin
	case(opcode)
		5'b00000: begin // no operation
			$display("ALU: NOP");
		end
		5'b00001: begin // and
			ALU_Result = Operand1 + Operand2;
			$display("ALU: R3 <- R2 + R1");
		end
		5'b00010: begin // and immediate
			ALU_Result = Operand1 + Operand2;
			$display("ALU: R3 <- R3 + immediate");
		end
		5'b00011: begin // or
			ALU_Result = Operand1 | Operand2;
			$display("ALU: R3 <- R2 or R1");
		end
		5'b00100: begin // or immediate
			ALU_Result = Operand1 | Operand2;
			$display("ALU: R3 <- R3 or immediate");
		end
		5'b00101: begin // xor
			ALU_Result = Operand1 ^ Operand2;
			$display("ALU: R3 <- R2 xor R1");
		end
		5'b00110: begin // xor immediate
			ALU_Result = Operand1 ^ Operand2;
			$display("ALU: R3 <- R3 xor immediate");
		end
		5'b00111: begin // nor
			ALU_Result = ~(Operand1 | Operand2);
			$display("ALU: R3 <- R2 nor R1");
		end
		5'b01000: begin // nor immediate
			ALU_Result = Operand1 | Operand2;
			$display("ALU: R3 <- R2 nor immediate");
		end
		5'b01001: begin // set less than
			ALU_Result = (Operand1 < Operand2)?(1):(0);
			$display("ALU: R3 <- is (R2 < R1)?");
		end
		5'b01010: begin // set less than immediate
			ALU_Result = (Operand1 < Operand2)?(1):(0);
			$display("ALU: R3 <- is (R3 < immediate)?");
		end
		5'b01011: begin // add
			ALU_Result = Operand1 + Operand2;
			$display("ALU: R3 <- R2 + R1");
		end
		5'b01100: begin // add immediate
			ALU_Result = Operand1 + Operand2;
			$display("ALU: R3 <- R3 add immediate");
		end
		5'b01101: begin // subtract
			ALU_Result = Operand1 - Operand2;
			$display("ALU: R3 <- R2 - R1");
		end
		5'b01110: begin // subtract immediate
			ALU_Result = Operand1 - Operand2;
			$display("ALU: R3 <- R3 - immediate");	
		end
		5'b01111: begin // branch if equal
			ALU_Result = Operand1 | Operand2;
			$display("ALU: R3 <- R2 nor immediate");
		end
		5'b10000: begin // branch if not equal
			ALU_Result = Operand1 | Operand2;
			$display("ALU: R3 <- R2 nor immediate");	
		end
		5'b10001: begin // shift left
			ALU_Result = Operand1 | Operand2;
			$display("ALU: R3 <- R2 nor immediate");
		end
		5'b10010: begin // shift right
			ALU_Result = Operand1 | Operand2;
			$display("ALU: R3 <- R2 nor immediate");
		end
		5'b10011: begin // not implemented
		
		end
		5'b10100: begin // not implemented
		
		end
		5'b10101: begin
		
		end
		5'b10110: begin
		
		end
		5'b10111: begin
		
		end
		5'b11000: begin // store
			$display("ALU: M[R3] <- R2");
			Store_OUT = 1;
		end
		5'b11001: begin // load
			$display("ALU: M[R3] -> R2");
			Load_OUT = 1;
		end
		5'b11010: begin // not implemented
		
		end
		5'b11011: begin // not implemented
		
		end
		5'b11100: begin // jump
		
		end
		5'b11101: begin // jump register
		
		end
		5'b11110: begin // jump register and link
		
		end
		5'b11111: begin //not implemented
		
		end
		default: begin //failsafe do nothing
			$display("ALU: opcode: DEFAULT");
		end
	endcase
endmodule
}
}

{
module Data_Memory(
	input CLK,
	input RESET,
	input [] Instruction_IN,
	input [] Program_Counter_IN,
	input [] ALU_Result_IN,
	input [] Result_Register_IN,
	input Load_IN,
	input Store_IN,
	input Writeback_IN,
	
	output [] TargetRegister_OUT,
	output [] WriteData_OUT,
	output Write2Register_OUT,
	output [] DataMemory_Address_OUT,
	output [] DataMemory_Data_OUT,
	input [] DataMemory_Data_IN
	
	/**
	input [] WriteRegister_IN
	input [] MemWriteData_IN,
	output [] MemRead_2DM,
	output [] MemWrite_2DM,
	output [] data_address_2DM,
	output reg [] TargetRegister_OUT,
	output reg [] WriteData_OUT,
	output reg Write2Register_OUT
	**/
)
	wire Load;
	wire Store;
	wire Writeback;
	wire MemoryData;
	wire MemReadAddress;
	wire MemWriteAddress;
	reg [] MemReadAddress;
	reg [] MemWriteAddress;
	
	assign ALU_Result = ALU_Result_IN;
	assign Load = Load_IN;
	assign Store = Store_IN;
	assign Writeback = Writeback_IN;


always @(Load) begin
	MemWriteAddress = ALU_Result
end

always @(posedge CLK or negedge RESET) begin
	if (!RESET) begin
		Instr1_OUT <= 0;
		Instr1_PC_OUT <= 0;
		WriteRegister1_OUT <= 0;
		RegWrite1_OUT <= 0;
		WriteData1_OUT <= 0;
	end else if (CLK) begin
		Instr1_OUT <= Instr1_IN;
		Instr1_PC_OUT <= Instr1_PC_IN;
		WriteRegister1_OUT <= WriteRegister1_IN;
		RegWrite1_OUT <= RegWrite1_IN;
		WriteData1_OUT <= WriteData1;
	end
	
endmodule
}

}
{
module Microprocessor(

)

Instruction_Fetch IF(
	.CLK(.CLK),
	.RESET(RESET),
	.Instruction_IN(Instruction),
	.Program_Counter_IN(Program_Counter),
	.Request_Alt_Program_Counter_IN(Request_Alt_Program_Counter),
	.Alternative_Program_Counter_IN(Alt_Program_Counter),
	.Instruction_OUT(Instruction),
	.Program_Counter_OUT(Program_Counter),
	.Next_Program_Counter_OUT(Next_Program_Counter),
);

Instruction_Decode ID(
	.CLK(.CLK),
	.RESET(RESET),
	.Instruction_IN(Instruction),
	.Program_Counter_IN(Program_Counter),
	.Request_Alt_Program_Counter_IN(Request_Alt_Program_Counter),
	.Alternative_Program_Counter_IN(Alt_Program_Counter),
	.Operand1_OUT(Operand1),
	.Operand2_OUT(Operand2),
	.SYS(SYS),
	// Register stuff
	.TargetRegister_IN(TargetRegister),
	.WriteData_IN(WriteData),
	.Write2Register_IN(Write2Register)
	
	
);

ALU EXE(
	.CLK(CLK),
	.Operand1_IN(Operand1),
	.Operand2_IN(Operand2),
	.Instruction_IN(Instruction),
	.ALU_Result_OUT(ALU_Result),
	.Result_Register_OUT(Result_Register),
	.Load_OUT(Load),
	.Store_OUT(Store),
	.Writeback_OUT(Writeback),
	//.Instruction_OUT(Instruction),
	Overflow(),
	Zero(),
	Negative()
);

Data_Memory MEM(
	.CLK(CLK),
	.RESET(RESET),
	.Instruction_IN(Instruction),
	.Program_Counter_IN(Program_Counter),
	.ALU_Result_IN(ALU_Result),
	.Result_Register_IN(Result_Register),
	.Load_IN(Load),
	.Store_IN(Store),
	.Writeback_IN(Writeback),

	// Writeback to register
	.TargetRegister_OUT(TargetRegister),
	.WriteData_OUT(WriteData),
	.Write2Register_OUT(Write2Register),
	// Interact with data memory
	.DataMemory_Address_OUT(),
	.DataMemory_Data_OUT(),
	//.DataMemory_WriteSize_OUT(),
	.DataMemory_Data_IN(),
)

endmodule
}