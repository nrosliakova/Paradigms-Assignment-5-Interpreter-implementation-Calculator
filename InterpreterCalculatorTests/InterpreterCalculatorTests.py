
import subprocess

def run_calculator(input_data):
    process = subprocess.Popen(
        #['InterpreterCalculator.exe'],
        #['C:\Dev\KSE\1 year\Principles and Paradigms\Assignments\Interpreter implementation Calculator\InterpreterCalculator\x64\Debug\InterpreterCalculator.exe'],
        ['C:\\Dev\\KSE\\1 year\\Principles and Paradigms\\Assignments\\Interpreter implementation Calculator\\InterpreterCalculator\\x64\\Debug\\InterpreterCalculator.exe'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, stderr = process.communicate(input_data)
    return stdout.strip(), stderr.strip()

def test_calculator():
    tests = [
         {
             "input": "1+2\n",
             "expected_output": "result = 3"
         },
         {
             "input": "max(3,7)\n",
             "expected_output": "result = 7"
         },
         {
             "input": "min(3,7)\n",
             "expected_output": "result = 3"
         },
         {
             "input": "m ax( mi n(7,10),  4)\n",
             "expected_output": "result = 7"
         },
         {
             "input": "min ( ma  x(7,10),  4)\n",
             "expected_output": "result = 4"
         },
         {
             "input": "min ( min(3,6),  4)\n",
             "expected_output": "result = 3"
         },
         {
             "input": "min ( min(3*5,6),  4+3)+10\n",
             "expected_output": "result = 16"
         },
         {
             "input": "2*m ax ( min(3*5,6),  4+3)+10\n",
             "expected_output": "result = 24"
         },
         {
             "input": "var a = 5\n6+a-2\n",
             "expected_output": "result = 9"
         },
         {
             "input": "var a = 5\nvar b = 2\na - b * 2\n",
             "expected_output": "result = 1"
         }
    ]

    for test in tests:
        input_data = test["input"]
        expected_output = test["expected_output"]

        output, error = run_calculator(input_data)

        if error:
            print(f"Test failed for input:\n{input_data}")
            print(f"Error:\n{error}")
        elif output.strip() != expected_output:
            print(f"Test failed for input:\n{input_data}")
            print(f"Expected:\n{expected_output}")
            print(f"Got:\n{output}")
        else:
            print(f"Test passed for input:\n{input_data}")

if __name__ == "__main__":
    test_calculator()