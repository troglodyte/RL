import tcod.console 
import tcod.context
import tcod.event
import tcod.tileset
from tcod import console

WIDTH, HEIGHT = 120, 60
FLAGS = tcod.context.SDL_WINDOW_RESIZABLE | tcod.context.SDL_WINDOW_MAXIMIZED

# https://www.roguebasin.com/index.php/Complete_Roguelike_Tutorial,_using_python%2Blibtcod,_part_1
# https://python-tcod.readthedocs.io/en/latest/tcod/getting-started.html


def main() -> None: 
    tileset = tcod.tileset.load_tilesheet(
            "dejavu10x10_gs_tc.png", 32, 8, tcod.tileset.CHARMAP_TCOD,
    )
    console = tcod.console.Console(WIDTH, HEIGHT)
    with tcod.context.new(  # New window for a console of size columns×rows.
            columns=console.width, rows=console.height, tileset=tileset,
    ) as context:
        while True:
            console = context.new_console(120,80)
            console.print(x=10, y=10, text="Hello World!", width=120, height=80)
            context.present(console, integer_scaling=True)

            for event in tcod.event.wait():
                context.convert_event(event)
                print(event)
                match event:
                    case tcod.event.Quit():
                        raise SystemExit
                    case tcod.event.WindowResized(width=width, height=height):
                        pass


if __name__ == '__main__':
    main()


