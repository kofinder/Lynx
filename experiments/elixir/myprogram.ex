# hello.exs
IO.write("Enter your name: ")
name = IO.gets("") |> String.trim()

IO.puts("Hello, #{name}!")
