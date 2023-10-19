import json
import sys

def combine_json(output_file, input_files):
    combined_data = {"converters": []}

    for input_file in input_files:
        with open(input_file, 'r') as f:
            data = json.load(f)
            combined_data["converters"].append(data)

    with open(output_file, 'w') as f:
        json.dump(combined_data, f, indent=2)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: combine_json.py output.json input1.json ...")
        sys.exit(1)

    output_file = sys.argv[1]
    input_files = sys.argv[2:]
    combine_json(output_file, input_files)
