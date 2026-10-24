import subprocess
import tkinter as tk
from pathlib import Path
from tkinter import messagebox


class SudokuGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Sudoku")

        # Allow the window to be resized
        self.root.resizable(True, True)
        self.root.geometry("850x750")
        self.root.minsize(450, 550)

        self.initial_values = []
        self.solver_path = self.find_solver()

        title = tk.Label(
            root,
            text="Sudoku",
            font=("Arial", 28, "bold")
        )
        title.pack(pady=(15, 5))

        main_frame = tk.Frame(root)
        main_frame.pack(
            padx=20,
            pady=15,
            fill="both",
            expand=True
        )

        # Allow the board column to expand
        main_frame.grid_rowconfigure(0, weight=1)
        main_frame.grid_columnconfigure(0, weight=1)

        # Black background creates the grid borders
        board_frame = tk.Frame(
            main_frame,
            bg="black",
            highlightbackground="black",
            highlightcolor="black",
            highlightthickness=2
        )
        board_frame.grid(
            row=0,
            column=0,
            sticky="nsew"
        )

        # Make every row and column expand evenly
        for row in range(9):
            board_frame.grid_rowconfigure(row, weight=1)

        for col in range(9):
            board_frame.grid_columnconfigure(col, weight=1)

        side_panel = tk.Frame(main_frame)
        side_panel.grid(
            row=0,
            column=1,
            padx=(25, 0),
            sticky="n"
        )

        self.entries = []

        # Slightly off-white background, approximately 5% darker than white
        board_color = "#F2F2F2"

        for row in range(9):
            entry_row = []

            for col in range(9):
                entry = tk.Entry(
                    board_frame,
                    font=("Arial", 20),
                    justify="center",
                    bg=board_color,
                    fg="#666666",
                    insertbackground="#666666",
                    disabledbackground=board_color,
                    disabledforeground="#111111",
                    relief="flat",
                    borderwidth=0,
                    highlightthickness=0
                )

                entry.grid(
                    row=row,
                    column=col,
                    sticky="nsew",
                    padx=(
                        4 if col % 3 == 0 else 1,
                        4 if col % 3 == 2 else 1
                    ),
                    pady=(
                        4 if row % 3 == 0 else 1,
                        4 if row % 3 == 2 else 1
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

        self.load_initial_board()

    def find_solver(self):
        """
        Find the C++ executable next to this Python file.
        Supports both Windows and extensionless executables.
        """
        program_directory = Path(__file__).resolve().parent

        possible_names = [
            "build.exe",
            "build",
        ]

        for name in possible_names:
            solver_path = program_directory / name

            if solver_path.is_file():
                return solver_path

        return None

    def run_cpp_program(self, arguments=None, input_data=None):
        if self.solver_path is None:
            messagebox.showerror(
                "Error",
                "Could not find the C++ executable.\n\n"
                "Make sure build.exe is in the same folder as sudoku_gui.py."
            )
            return None

        if arguments is None:
            arguments = []

        try:
            return subprocess.run(
                [str(self.solver_path)] + arguments,
                input=input_data,
                text=True,
                capture_output=True,
                cwd=self.solver_path.parent
            )

        except OSError as error:
            messagebox.showerror(
                "Error",
                f"Could not run the C++ program:\n\n{error}"
            )
            return None

    def set_cell(self, row, col, value, disabled=False):
        entry = self.entries[row][col]

        # Disabled entries must temporarily be enabled before editing
        entry.configure(state="normal")
        entry.delete(0, tk.END)

        if value != "0":
            entry.insert(0, value)

        if disabled:
            # Initial values are dark and cannot be changed
            entry.configure(state="disabled")
        else:
            # User-entered and solved values are gray
            entry.configure(
                fg="#666666",
                insertbackground="#666666"
            )

    def load_initial_board(self):
        result = self.run_cpp_program(["--initial"])

        if result is None:
            return

        if result.returncode != 0:
            messagebox.showerror(
                "Error",
                "Could not load the initial board.\n\n"
                + result.stderr
            )
            return

        values = result.stdout.split()

        if len(values) != 81:
            messagebox.showerror(
                "Error",
                "The C++ program returned an invalid board."
            )
            return

        self.initial_values = values

        index = 0

        for row in range(9):
            for col in range(9):
                value = values[index]

                if value != "0":
                    self.set_cell(
                        row,
                        col,
                        value,
                        disabled=True
                    )

                index += 1

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

        # Send the current board to the C++ program.
        # The C++ program should read these 81 values from stdin
        # and print the solved board as 81 space-separated values.
        input_data = " ".join(values)

        result = self.run_cpp_program(
            input_data=input_data
        )

        if result is None:
            return

        if result.returncode != 0:
            messagebox.showerror(
                "No solution",
                "The puzzle is invalid or unsolvable.\n\n"
                + result.stderr
            )
            return

        solved_values = result.stdout.split()

        if len(solved_values) != 81:
            messagebox.showerror(
                "Error",
                "The C++ solver returned invalid output."
            )
            return

        # Display the values returned by the C++ solver
        index = 0

        for row in range(9):
            for col in range(9):
                is_initial = self.initial_values[index] != "0"

                self.set_cell(
                    row,
                    col,
                    solved_values[index],
                    disabled=is_initial
                )

                index += 1

    def reset(self):
        index = 0

        for row in range(9):
            for col in range(9):
                value = self.initial_values[index]
                is_initial = value != "0"

                self.set_cell(
                    row,
                    col,
                    value,
                    disabled=is_initial
                )

                index += 1


root = tk.Tk()
SudokuGUI(root)
root.mainloop()