import subprocess
import tkinter as tk
from tkinter import messagebox


class SudokuGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Sudoku Solver")

        main_frame = tk.Frame(root)
        main_frame.pack(padx=20, pady=20)

        board_frame = tk.Frame(main_frame)
        board_frame.grid(row=0, column=0)

        side_panel = tk.Frame(main_frame)
        side_panel.grid(row=0, column=1, padx=(25, 0), sticky="n")

        self.entries = []

        for row in range(9):
            entry_row = []

            for col in range(9):
                entry = tk.Entry(
                    board_frame,
                    width=2,
                    font=("Arial", 20),
                    justify="center"
                )

                entry.grid(
                    row=row,
                    column=col,
                    padx=(
                        (4 if col % 3 == 0 else 1),
                        (4 if col == 8 else 1)
                    ),
                    pady=(
                        (4 if row % 3 == 0 else 1),
                        (4 if row == 8 else 1)
                    )
                )

                entry_row.append(entry)

            self.entries.append(entry_row)

        solve_button = tk.Button(
            side_panel,
            text="Solve",
            width=12,
            command=self.solve
        )
        solve_button.pack(pady=5)

        reset_button = tk.Button(
            side_panel,
            text="Reset",
            width=12,
            command=self.reset
        )
        reset_button.pack(pady=5)

    def solve(self):
        values = []

        try:
            for row in range(9):
                for col in range(9):
                    text = self.entries[row][col].get().strip()

                    if text == "":
                        values.append("0")
                    else:
                        value = int(text)

                        if value < 1 or value > 9:
                            raise ValueError

                        values.append(str(value))

        except ValueError:
            messagebox.showerror(
                "Invalid input",
                "Enter only numbers from 1 to 9."
            )
            return

        input_data = " ".join(values)

        result = subprocess.run(
            ["./build"],
            input=input_data,
            text=True,
            capture_output=True
        )

        if result.returncode != 0:
            messagebox.showerror(
                "No solution",
                "The puzzle is invalid or unsolvable."
            )
            return

        solved_values = result.stdout.split()

        if len(solved_values) != 81:
            messagebox.showerror(
                "Error",
                "The C++ solver returned invalid output."
            )
            return

        index = 0

        for row in range(9):
            for col in range(9):
                self.entries[row][col].delete(0, tk.END)
                self.entries[row][col].insert(
                    0,
                    solved_values[index]
                )
                index += 1

    def reset(self):
        for row in self.entries:
            for entry in row:
                entry.delete(0, tk.END)


root = tk.Tk()
SudokuGUI(root)
root.mainloop()
