import subprocess

def run_interpreter(input_data):
    process = subprocess.Popen(
        ['./interpreter'],  # Path to the compiled C++ program
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, stderr = process.communicate(input_data)
    return stdout.strip(), stderr.strip()

def test_interpreter():
    tests = [
        {"input": "7*6\nexit\n", "expected_output": "42"},
        {"input": "var a=6*2\na+8\nexit\n", "expected_output": "20"},
        {"input": "-1+3\nexit\n", "expected_output": "2"},
        {"input": "max(min(3*2,4),2)\nexit\n", "expected_output": "4"},
        {"input": "abs(-13)\nexit\n", "expected_output": "13"},
        {"input": "pow(min(2,3),max(3,4))\nexit\n", "expected_output": "16"},


    ]

    for test in tests:
        input_data = test["input"]
        expected_output = test["expected_output"]
        output, error = run_interpreter(input_data)

        if error:
            print(f"Test failed for input:\n{input_data}")
            print(f"Error:\n{error}")
        elif output != expected_output:
            print(f"Test failed for input:\n{input_data}")
            print(f"Expected:\n{expected_output}")
            print(f"Got:\n{output}")
        else:
            print(f"Test passed for input:\n{input_data}")

if __name__ == "__main__":
    test_interpreter()
