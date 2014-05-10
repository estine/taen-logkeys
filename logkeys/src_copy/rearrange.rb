#!/usr/bin/ruby

class Rearrange

  def initialize(file_name)
    @file = file_name
  end
  
  def get_methods
    method_names ||= methods
    
  end

  def test_methods
    methods
  end


  private
  def methods
    methods = Hash.new
    IO.foreach(@file) do |f|
      methods["#{f}".strip] = "" if /^\w+\s\w+_\w+\(\)/.match f
    end
    methods
  end
end
