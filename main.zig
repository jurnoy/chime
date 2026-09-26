const rl = @import("raylib");
const std = @import("std");

const FONT_COLOR: rl.Color = .{ .r = 0xe8, .g = 0xe6, .b = 0xe3, .a = 0xff };
const BACKROUND_COLOR: rl.Color = .{ .r = 0x18, .g = 0x1a, .b = 0x1b, .a = 0xff };
const WRONG_COLOR: rl.Color = .{ .r = 0x5d, .g = 0x64, .b = 0x68, .a = 0xff };
const MISPLACED_COLOR: rl.Color = .{ .r = 0x68, .g = 0x5b, .b = 0x22, .a = 0xff };
const CORRECT_COLOR: rl.Color = .{ .r = 0x57, .g = 0x7e, .b = 0x45, .a = 0xff };
const OUTLINE_COLOR: rl.Color = .{ .r = 0x3b, .g = 0x40, .b = 0x43, .a = 0xff };

const SQ_SIZE: f32 = 140;

const Game_Stage = enum {
    START,
    WORDLE,
    END
};

const StartMenu = struct {
    filename: *std.ArrayList(u8),
    allocator: std.mem.Allocator,
    change: bool,

    pub fn init(filename: *std.ArrayList(u8), allocator: std.mem.Allocator) StartMenu {
        
    }
    
    // pub fn draw(self: StartMenu) void {
        
    // }
};

fn DrawStartMenu(file_name: *std.ArrayList(u8), allocator: std.mem.Allocator) !bool {
    rl.clearBackground(.white);
    rl.drawRectangle(SQ_SIZE * 1.5, SQ_SIZE * 2.5, SQ_SIZE * 3, SQ_SIZE / 2, .beige);

    while (true) {
        const c = rl.getCharPressed();
        if (c == 0) break;

        switch (c) {
            'A'...'Z' => {
                try file_name.append(allocator, @as(u8, @intCast(c)));
                std.debug.print("{c}", .{@as(u8, @intCast(c))});
            },
            'a'...'z' => {
                try file_name.append(allocator, @as(u8, @intCast(c)));
                std.debug.print("{c}", .{@as(u8, @intCast(c))});
            },
            '.' => {
                try file_name.append(allocator, '.');
                std.debug.print("{c}", .{'.'});
            },
            else => {},
        }
    }
    
    if (rl.isKeyPressed(.enter)) {
        return true;
    }

    return false;
}

const Block = struct {
    color: rl.Color,
    letter: u8,
    pos: rl.Vector2,
    outline: bool,

    fn init(pos: rl.Vector2) Block {
        return .{ .color = OUTLINE_COLOR, .pos = pos, .letter = 0, .outline = true };
    }

    fn draw(self: Block) void {
        self.drawBox();
        if (self.letter != 0) {
            self.write(self.letter);
        }
    }

    fn reset(self: *Block) void {
        self.color = OUTLINE_COLOR;
        self.letter = 0;
        self.outline = true;
    }

    fn change_color(self: *Block, color: rl.Color) void {
        self.color = color;
        self.outline = false;
    }

    fn write(self: Block, letter: u8) void {
        rl.drawText(&[_:0]u8{letter}, @ceil(self.pos.x + ((SQ_SIZE - 55) / 2)), @ceil(self.pos.y + ((SQ_SIZE - 55) / 2)), 55, FONT_COLOR);
    }

    fn drawBox(self: Block) void {
        const rectangle_vec = rl.Vector2.init(SQ_SIZE, SQ_SIZE);
        rl.drawRectangleV(self.pos, rectangle_vec, BACKROUND_COLOR);
        rl.drawRectangleV(self.pos.addValue(SQ_SIZE * 0.05), rectangle_vec.scale(0.9), self.color);
        if (self.outline == true) {
            rl.drawRectangleV(self.pos.addValue(SQ_SIZE * 0.1), rectangle_vec.scale(0.8), BACKROUND_COLOR);
        }
    }
};

const Correctness = enum {
    CORRECT,
    MISPLACED,
    WRONG,
};

fn lower_char(c: u8) !u8 {
    return switch (c) {
        'A'...'Z' => c + 32,
        'a'...'z' => c,
        else => error.NotValidCharacter,
    };
}

fn upper_char(c: u8) !u8 {
    return switch (c) {
        'A'...'Z' => c,
        'a'...'z' => c - 32,
        else => error.NotValidCharacter,
    };
}

pub fn check(answer: []const u8, guess: []const u8) [5]Correctness {
    var points: [5]Correctness = undefined;
    @memset(points[0..], Correctness.WRONG);
    var mask: [5]u8 = undefined;
    @memset(mask[0..], 0);

    for (guess, answer, 0..) |char, cchar, i| {
        if (char == cchar) {
            points[i] = Correctness.CORRECT;
            mask[i] = 1;
        }
    }

    for (guess, 0..) |char, i| {
        if (points[i] == Correctness.CORRECT) {
            continue;
        }
        for (answer, 0..) |cchar, n| {
            if ((mask[n] == 0) and (char == cchar)) {
                points[i] = Correctness.MISPLACED;
                mask[n] = 1;
            }
        }
    }

    return points;
}

fn updateBlocksColors(block_list: []Block, points: []const Correctness) bool {
    var end = true;
    for (block_list, points) |*block, point| {
        switch (point) {
            Correctness.CORRECT => {
                block.change_color(CORRECT_COLOR);
            },
            Correctness.MISPLACED => {
                block.change_color(MISPLACED_COLOR);
                end = false;
            },
            Correctness.WRONG => {
                block.change_color(WRONG_COLOR);
                end = false;
            },
        }
    }

    return end;
}

fn load_words(io: std.Io, path: []const u8, allocator: std.mem.Allocator) ![][]u8 {
    const cwd = std.Io.Dir.cwd();
    const file = try cwd.openFile(io, path, .{ .mode = .read_only });
    defer file.close(io);

    const file_length = try file.length(io);
    const total_words = try std.math.divFloor(u64, file_length, 5 + 1);

    var file_contents = try allocator.alloc(u8, file_length);
    @memset(file_contents[0..], 0);
    _ = try file.readPositionalAll(io, file_contents, 0);

    var words = try allocator.alloc([]u8, total_words);
    var start_index: u64 = 0;
    for (0..total_words) |i| {
        words[i] = file_contents[start_index..(start_index + 5)];
        start_index += 5 + 1;
    }

    return words;
}

pub fn main(init: std.process.Init) !void {
    // Backend Initialization
    // --------------------------------------------------------------------------------------
    
    var stage: Game_Stage = Game_Stage.START;
    
    const io = init.io;
    var gpa = std.heap.DebugAllocator(.{}){};
    var aa = std.heap.ArenaAllocator.init(gpa.allocator());
    const allocator = aa.allocator();
    defer aa.deinit();

    var file_name = try std.ArrayList(u8).initCapacity(allocator, 1024);


    // Initialization
    //--------------------------------------------------------------------------------------
    const screenWidth = SQ_SIZE * 5;
    const screenHeight = SQ_SIZE * 6;

    var blocks_list: [30]Block = undefined;

    for (0..6) |y| {
        for (0..5) |x| {
            const pos_vec = rl.Vector2.init(@as(f32, @floatFromInt(x)) * SQ_SIZE, @as(f32, @floatFromInt(y)) * SQ_SIZE);
            blocks_list[y * 5 + x] = Block.init(pos_vec);
        }
    }

    rl.initWindow(screenWidth, screenHeight, "W");
    defer rl.closeWindow(); // Close window and OpenGL context

    rl.setTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    var text: [5]u8 = [_]u8{0} ** 5;
    var turn: u8 = 0;
    var index: u8 = 0;
    var correct = [_]u8{0} ** 5;
    
    var game_on = true;
    
    // Main game loop
    while (!rl.windowShouldClose()) { // Detect window close button or ESC key
        // Draw
        //----------------------------------------------------------------------------------
        rl.beginDrawing();
        defer rl.endDrawing();
        if (stage == Game_Stage.START) {
            const change = try DrawStartMenu(&file_name, allocator);
            if (change) {
                stage = Game_Stage.WORDLE;
                const file = try file_name.toOwnedSlice(allocator);
                const words = try load_words(io, file, allocator);
            
                var randSource: std.Random.IoSource = .{ .io = io };
                const rand = randSource.interface();
            
                @memcpy(&correct, words[rand.uintAtMost(usize, words.len)][0..5]);
                std.debug.print("Answer: {s}\n", .{correct});
            }
        }
        
        if (stage == Game_Stage.WORDLE) {
            if (game_on) {
                while (true) {
                    const char = rl.getCharPressed();
                    if (char == 0) break;
    
                    if (index < 5) {
                        const c = switch (char) {
                            'A'...'Z' => try lower_char(@as(u8, @intCast(char))),
                            'a'...'z' => @as(u8, @intCast(char)),
                            else => null
                        };
                        if (c != null) {
                            text[index] = c.?;
                            blocks_list[(turn * 5) + index].letter = try upper_char(c.?);
                            index += 1;
                        }
                    }
                }
    
                if (rl.isKeyPressed(.backspace) and index != 0) {
                    index -= 1;
                    text[index] = 0;
                    blocks_list[(turn * 5) + index].letter = 0;
                }
                if (rl.isKeyPressed(.enter) and index >= 5) {
                    const points = check(&correct, text[0..]);
                    const end = updateBlocksColors(blocks_list[turn * 5 .. (turn + 1) * 5], &points);
                    index = 0;
                    text = [_]u8{0} ** 5;
                    turn += 1;
                    if (turn >= 6 or end == true) {
                        game_on = false;
                    }
                }
            }
    
            if (rl.isKeyPressed(.delete)) {
                game_on = true;
                index = 0;
                turn = 0;
                for (&blocks_list) |*block| {
                    block.reset();
                }
                // correct = words[rand.uintLessThan(usize, words.len)];
                std.debug.print("Answer: {s}\n", .{correct});
            }
    
            for (blocks_list) |block| {
                block.draw();
            }
        }
        //----------------------------------------------------------------------------------
    }
}
