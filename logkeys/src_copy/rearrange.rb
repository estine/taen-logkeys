#!/usr/bin/ruby

class Rearrange

  def initialize(file_name)
    @file = file_name
  end

  def get_methods
    method_names ||= methods
    method_names << "int main"
    methods_hash = Hash.new
    count = 1
    method_names.each do |method_name|
      temp = ""
      IO.foreach(@file) do |line|
        # puts "apples"if line.match(/void signal_handler\(int signal\)/)
        if line.match(/#{method_name}/) ... line.match(/#{method_names[count].to_s}/)
          # puts method_name
          # puts method_names[count].to_s
          temp << line.gsub(/#{method_names[count].to_s}\(.*\)/, "")
        end
        methods_hash["#{method_name}"] = temp
        # method_names.shift
      end
      count += 1
    end
    methods_hash = delete_unwanted_methods(methods_hash)
  end

  def get_first_half
    first_half = ""
    IO.foreach(@file) do |line|
      if line.match(/\/\*/) .. line.match(/int input_fd/)
        first_half << line
      end
    end
    first_half
  end

  def get_last_half
    last_half = ""
    IO.foreach(@file) do |line|
      if line.match(/int main/) .. line.match(/\A\/\/\send/)
        last_half << line
      end
    end
    last_half
  end

  def create_new_source_file
    source = get_first_half

    methods = get_methods
    declarations = methods.keys
    declarations.shuffle!
    declarations.each do |key|
      source << methods[key]
    end

    puts source << get_last_half
  end

  def test_methods
    methods
  end


  private
  def delete_unwanted_methods(hash)
    hash.delete("int main")
    hash
  end
  def methods
    methods = Array.new
    IO.foreach(@file) do |f|
      methods << f.strip.gsub(/\(.*\)/, "") if /^\w+\s\w+_\w+\(.*\)/.match f
    end
    methods
  end
end
