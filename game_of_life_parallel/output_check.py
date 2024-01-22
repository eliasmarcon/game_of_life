import sys

def compare_text_files(file1_path, file2_path, number, mode):
    try:
        with open(file1_path, 'r') as file1, open(file2_path, 'r') as file2:
            content1 = file1.read()
            content2 = file2.read()

            if content1 == content2:
                print(f"The text files for {number:>5} with {mode} run are exactly the same.")
            else:
                print("The text files are not the same.")
    except FileNotFoundError:
        print("One or both of the files were not found.")
    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":

    input_mode = sys.argv[1]
    print("\n")

    for i in range(1000, 10001, 1000):

        file3_path = f'../step1000_out_250generations/random{i}_out.gol'
        
        if input_mode == "seq":
            file1_path = f'./output_files/ai23m020_{i}_cpu_out.txt'
            compare_text_files(file1_path, file3_path, i, "cpu")
        
        elif input_mode == "omp":
            file2_path = f'./output_files/ai23m020_{i}_openmp_out.txt'
            compare_text_files(file2_path, file3_path, i, "openmp")
        
        else:
            break

