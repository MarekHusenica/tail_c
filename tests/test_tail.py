import subprocess
import pytest
import os

TAIL_BIN="./tail"

@pytest.fixture
def create_file(tmp_path):
    def _create(name, lines):
        f = tmp_path / name
        content = "\n".join([f"Line {i}" for i in range(1, lines + 1)])
        f.write_text(content + "\n")
        return str(f)
    return _create


def test_default_ten_lines(create_file):
    path = create_file("test15.txt", 15)
    result = subprocess.run([TAIL_BIN, path], capture_output=True, text=True)

    lines = result.stdout.strip().split('\n')
    assert len(lines) == 10
    assert lines[0] == "Line 6"
    assert lines[-1] == "Line 15"

def test_file_shorter_than_ten(create_file):
    path = create_file("test5.txt", 5)
    result = subprocess.run([TAIL_BIN, path], capture_output=True, text=True)

    lines = result.stdout.strip().split('\n')
    assert len(lines) == 5
    assert lines[0] == "Line 1"


def test_empty_file(tmp_path):
    empty = tmp_path / "empty.txt"
    empty.write_text("")

    result = subprocess.run([TAIL_BIN, str(empty)], capture_output=True, text=True)
    assert result.stdout == ""
    assert result.returncode == 0

def test_file_not_found():
    result = subprocess.run([TAIL_BIN, "no_file.txt"], capture_output=True, text=True)

    assert result.returncode  != 0
    assert len(result.stderr) > 0


@pytest.mark.parametrize("line_count", [1, 10, 50, 100])
def test_compare_to_sys_tail(create_file, line_count):
    path = create_file(f"compare_{line_count}.txt", line_count)

    my_result = subprocess.run([TAIL_BIN, path], capture_output=True, text=True)
    sys_result = subprocess.run(["tail", path], capture_output=True, text=True)

    assert my_result.stdout == sys_result.stdout
