def compare_text_files(file1_path, file2_path, number):
    try:
        with open(file1_path, 'r') as file1, open(file2_path, 'r') as file2:
            content1 = file1.read()
            content2 = file2.read()

            if content1 == content2:
                print(f"The text files for {number} run are exactly the same.")
            else:
                print("The text files are not the same.")
    except FileNotFoundError:
        print("One or both of the files were not found.")
    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":

    for i in range(1000, 10001, 1000):

        file1_path = f'./output_files/ai23m020_{i}_cpu_out.txt'
        file2_path = f'../step1000_out_250generations/random{i}_out.gol'
        compare_text_files(file1_path, file2_path, i)