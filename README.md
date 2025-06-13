Simple TODO that will:  
 - Records items and removes them
 - Records done items (Motivation to get things done!)
 - 'Alarm system' that reminds you of work to be done
 - Reminds one also to set items to be done.

Will use it in Awesome window manager to display the tasks, one at a time.

-----------------
Awesome WM shows the items in the winbar, but anywhere works!
Awesome's WM rc.lua would look smth like:
<pre>
-- Create the textbox widget
local todo_widget = wibox.widget.textbox()

-- Function to update the todo list
local function update_todo_list()
    local f = io.open(todo_file, "r")
    local todo_content = ""
    if f then
        for line in f:lines() do
            todo_content = todo_content .. line .. "\n"
        end
        f:close()
    else
        todo_content = "No todo file found!"
    end
    todo_widget.text = todo_content
end


-- Initial update
update_todo_list()

-- Set up a timer to update the todo list periodically
gears.timer {
    timeout = 60, -- Update every 60 seconds
    call_now = false,
    autostart = true,
    callback = function() update_todo_list() end
}

  -- call the widget in s.mywibox:setup block
</pre>
