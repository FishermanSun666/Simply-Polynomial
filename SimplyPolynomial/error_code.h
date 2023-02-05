#pragma once

const enum ErrorCode {
	PROCESS_ERROR,
	SOLVE_EXAMPLE_ERROR,
	INPUT_ERROR,
	OUTPUT_IS_EMPTY,
	EXPERSSION_NOT_COMPLETE,
	WRITE_FILE_ERROR,
};

void ExceptionHandling(int code);

