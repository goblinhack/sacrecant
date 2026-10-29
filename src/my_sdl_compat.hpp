//
// Copyright goblinhack@gmail.com
//
// SDL2 / SDL3 compatibility.
//
// The game is written against the SDL2 API. Define USE_SDL3 to build against SDL3 instead; the default is
// SDL2 and in that case everything below is a thin pass through (or nothing at all).
//
// For SDL3 most of this file is renames, so that the game code can carry on using the SDL2 names. Where
// the behaviour or the signature differs (return types, float coordinates, key events, display modes etc...)
// there is a small sdl_compat_xxx() wrapper that does the right thing for either version.
//
// Parts of the game that differ more fundamentally, such as audio, joysticks and event handling, have
// #ifdef USE_SDL3 blocks in the files themselves.
//

#ifndef MY_SDL_COMPAT_HPP
#define MY_SDL_COMPAT_HPP

#include <cstdint>

#ifdef USE_SDL3

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

//
// SDL3_mixer has no fixed maximum volume; we map our 0..MIX_MAX_VOLUME range onto a float gain.
//
#ifndef MIX_MAX_VOLUME
#define MIX_MAX_VOLUME 128
#endif

//
// SDL3_mixer reports errors via SDL_GetError
//
#define Mix_GetError SDL_GetError

//
// Keycodes and scancodes that exist in SDL2 but have been removed from SDL3. Give them values that
// no real key will ever produce, so they never match and the SDL2 style lookup tables still compile.
//
#define SDL_COMPAT_NO_KEYCODE  (static_cast< SDL_Keycode >(0x7ffffff0))
#define SDL_COMPAT_NO_SCANCODE (static_cast< SDL_Scancode >(0x7ff0))

#undef AUDIO_F32
#undef AUDIO_F32LSB
#undef AUDIO_F32MSB
#undef AUDIO_F32SYS
#undef AUDIO_S16
#undef AUDIO_S16LSB
#undef AUDIO_S16MSB
#undef AUDIO_S16SYS
#undef AUDIO_S32
#undef AUDIO_S32LSB
#undef AUDIO_S32MSB
#undef AUDIO_S32SYS
#undef AUDIO_S8
#undef AUDIO_U8
#undef KMOD_ALT
#undef KMOD_CAPS
#undef KMOD_CTRL
#undef KMOD_GUI
#undef KMOD_LALT
#undef KMOD_LCTRL
#undef KMOD_LGUI
#undef KMOD_LSHIFT
#undef KMOD_MODE
#undef KMOD_NONE
#undef KMOD_NUM
#undef KMOD_RALT
#undef KMOD_RCTRL
#undef KMOD_RGUI
#undef KMOD_RSHIFT
#undef KMOD_SCROLL
#undef KMOD_SHIFT
#undef RW_SEEK_CUR
#undef RW_SEEK_END
#undef RW_SEEK_SET
#undef SDL_AllocFormat
#undef SDL_AllocPalette
#undef SDL_AndroidBackButton
#undef SDL_AndroidGetActivity
#undef SDL_AndroidGetExternalStoragePath
#undef SDL_AndroidGetExternalStorageState
#undef SDL_AndroidGetInternalStoragePath
#undef SDL_AndroidGetJNIEnv
#undef SDL_AndroidRequestPermission
#undef SDL_AndroidRequestPermissionCallback
#undef SDL_AndroidSendMessage
#undef SDL_AndroidShowToast
#undef SDL_APP_DIDENTERBACKGROUND
#undef SDL_APP_DIDENTERFOREGROUND
#undef SDL_APP_LOWMEMORY
#undef SDL_APP_TERMINATING
#undef SDL_APP_WILLENTERBACKGROUND
#undef SDL_APP_WILLENTERFOREGROUND
#undef SDL_atomic_t
#undef SDL_AtomicAdd
#undef SDL_AtomicCAS
#undef SDL_AtomicCASPtr
#undef SDL_AtomicGet
#undef SDL_AtomicGetPtr
#undef SDL_AtomicLock
#undef SDL_AtomicSet
#undef SDL_AtomicSetPtr
#undef SDL_AtomicTryLock
#undef SDL_AtomicUnlock
#undef SDL_AUDIODEVICEADDED
#undef SDL_AUDIODEVICEREMOVED
#undef SDL_AudioStreamAvailable
#undef SDL_AudioStreamClear
#undef SDL_AudioStreamFlush
#undef SDL_AudioStreamGet
#undef SDL_AudioStreamPut
#undef SDL_BlitScaled
#undef SDL_bool
#undef SDL_BUTTON
#undef SDL_CLIPBOARDUPDATE
#undef SDL_Colour
#undef SDL_COMPILEDVERSION
#undef SDL_cond
#undef SDL_CondBroadcast
#undef SDL_CondSignal
#undef SDL_CondWait
#undef SDL_CondWaitTimeout
#undef SDL_CONTROLLER_AXIS_INVALID
#undef SDL_CONTROLLER_AXIS_LEFTX
#undef SDL_CONTROLLER_AXIS_LEFTY
#undef SDL_CONTROLLER_AXIS_MAX
#undef SDL_CONTROLLER_AXIS_RIGHTX
#undef SDL_CONTROLLER_AXIS_RIGHTY
#undef SDL_CONTROLLER_AXIS_TRIGGERLEFT
#undef SDL_CONTROLLER_AXIS_TRIGGERRIGHT
#undef SDL_CONTROLLER_BINDTYPE_AXIS
#undef SDL_CONTROLLER_BINDTYPE_BUTTON
#undef SDL_CONTROLLER_BINDTYPE_HAT
#undef SDL_CONTROLLER_BINDTYPE_NONE
#undef SDL_CONTROLLER_BUTTON_A
#undef SDL_CONTROLLER_BUTTON_B
#undef SDL_CONTROLLER_BUTTON_BACK
#undef SDL_CONTROLLER_BUTTON_DPAD_DOWN
#undef SDL_CONTROLLER_BUTTON_DPAD_LEFT
#undef SDL_CONTROLLER_BUTTON_DPAD_RIGHT
#undef SDL_CONTROLLER_BUTTON_DPAD_UP
#undef SDL_CONTROLLER_BUTTON_GUIDE
#undef SDL_CONTROLLER_BUTTON_INVALID
#undef SDL_CONTROLLER_BUTTON_LEFTSHOULDER
#undef SDL_CONTROLLER_BUTTON_LEFTSTICK
#undef SDL_CONTROLLER_BUTTON_MAX
#undef SDL_CONTROLLER_BUTTON_MISC1
#undef SDL_CONTROLLER_BUTTON_PADDLE1
#undef SDL_CONTROLLER_BUTTON_PADDLE2
#undef SDL_CONTROLLER_BUTTON_PADDLE3
#undef SDL_CONTROLLER_BUTTON_PADDLE4
#undef SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
#undef SDL_CONTROLLER_BUTTON_RIGHTSTICK
#undef SDL_CONTROLLER_BUTTON_START
#undef SDL_CONTROLLER_BUTTON_TOUCHPAD
#undef SDL_CONTROLLER_BUTTON_X
#undef SDL_CONTROLLER_BUTTON_Y
#undef SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT
#undef SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR
#undef SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT
#undef SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO
#undef SDL_CONTROLLER_TYPE_PS3
#undef SDL_CONTROLLER_TYPE_PS4
#undef SDL_CONTROLLER_TYPE_PS5
#undef SDL_CONTROLLER_TYPE_UNKNOWN
#undef SDL_CONTROLLER_TYPE_VIRTUAL
#undef SDL_CONTROLLER_TYPE_XBOX360
#undef SDL_CONTROLLER_TYPE_XBOXONE
#undef SDL_ControllerAxisEvent
#undef SDL_CONTROLLERAXISMOTION
#undef SDL_CONTROLLERBUTTONDOWN
#undef SDL_ControllerButtonEvent
#undef SDL_CONTROLLERBUTTONUP
#undef SDL_CONTROLLERDEVICEADDED
#undef SDL_ControllerDeviceEvent
#undef SDL_CONTROLLERDEVICEREMAPPED
#undef SDL_CONTROLLERDEVICEREMOVED
#undef SDL_ControllerSensorEvent
#undef SDL_CONTROLLERSENSORUPDATE
#undef SDL_CONTROLLERSTEAMHANDLEUPDATED
#undef SDL_CONTROLLERTOUCHPADDOWN
#undef SDL_ControllerTouchpadEvent
#undef SDL_CONTROLLERTOUCHPADMOTION
#undef SDL_CONTROLLERTOUCHPADUP
#undef SDL_ConvertSurfaceFormat
#undef SDL_CreateCond
#undef SDL_DelEventWatch
#undef SDL_DelHintCallback
#undef SDL_DestroyCond
#undef SDL_Direct3D9GetAdapterIndex
#undef SDL_DISPLAYEVENT_CONNECTED
#undef SDL_DISPLAYEVENT_DISCONNECTED
#undef SDL_DISPLAYEVENT_MOVED
#undef SDL_DISPLAYEVENT_ORIENTATION
#undef SDL_DROPBEGIN
#undef SDL_DROPCOMPLETE
#undef SDL_DROPFILE
#undef SDL_DROPTEXT
#undef SDL_DXGIGetOutputInfo
#undef SDL_EncloseFPoints
#undef SDL_EnclosePoints
#undef SDL_eventaction
#undef SDL_FALSE
#undef SDL_FillRect
#undef SDL_FillRects
#undef SDL_FINGERDOWN
#undef SDL_FINGERMOTION
#undef SDL_FINGERUP
#undef SDL_FIRSTEVENT
#undef SDL_FRectEmpty
#undef SDL_FRectEquals
#undef SDL_FRectEqualsEpsilon
#undef SDL_FreeAudioStream
#undef SDL_FreeCursor
#undef SDL_FreePalette
#undef SDL_FreeSurface
#undef SDL_FreeWAV
#undef SDL_GameController
#undef SDL_GameControllerAddMapping
#undef SDL_GameControllerAddMappingsFromFile
#undef SDL_GameControllerAddMappingsFromRW
#undef SDL_GameControllerAxis
#undef SDL_GameControllerBindType
#undef SDL_GameControllerButton
#undef SDL_GameControllerClose
#undef SDL_GameControllerFromInstanceID
#undef SDL_GameControllerFromPlayerIndex
#undef SDL_GameControllerGetAppleSFSymbolsNameForAxis
#undef SDL_GameControllerGetAppleSFSymbolsNameForButton
#undef SDL_GameControllerGetAttached
#undef SDL_GameControllerGetAxis
#undef SDL_GameControllerGetAxisFromString
#undef SDL_GameControllerGetButton
#undef SDL_GameControllerGetButtonFromString
#undef SDL_GameControllerGetFirmwareVersion
#undef SDL_GameControllerGetJoystick
#undef SDL_GameControllerGetNumTouchpadFingers
#undef SDL_GameControllerGetNumTouchpads
#undef SDL_GameControllerGetPlayerIndex
#undef SDL_GameControllerGetProduct
#undef SDL_GameControllerGetProductVersion
#undef SDL_GameControllerGetSensorData
#undef SDL_GameControllerGetSensorDataRate
#undef SDL_GameControllerGetSerial
#undef SDL_GameControllerGetSteamHandle
#undef SDL_GameControllerGetStringForAxis
#undef SDL_GameControllerGetStringForButton
#undef SDL_GameControllerGetTouchpadFinger
#undef SDL_GameControllerGetType
#undef SDL_GameControllerGetVendor
#undef SDL_GameControllerHasAxis
#undef SDL_GameControllerHasButton
#undef SDL_GameControllerHasSensor
#undef SDL_GameControllerIsSensorEnabled
#undef SDL_GameControllerMapping
#undef SDL_GameControllerMappingForDeviceIndex
#undef SDL_GameControllerMappingForGUID
#undef SDL_GameControllerName
#undef SDL_GameControllerOpen
#undef SDL_GameControllerPath
#undef SDL_GameControllerRumble
#undef SDL_GameControllerRumbleTriggers
#undef SDL_GameControllerSendEffect
#undef SDL_GameControllerSetLED
#undef SDL_GameControllerSetPlayerIndex
#undef SDL_GameControllerSetSensorEnabled
#undef SDL_GameControllerType
#undef SDL_GameControllerUpdate
#undef SDL_GDKGetDefaultUser
#undef SDL_GDKGetTaskQueue
#undef SDL_GetClipRect
#undef SDL_GetClosestDisplayMode
#undef SDL_GetColorKey
#undef SDL_GetCPUCount
#undef SDL_GetDisplayOrientation
#undef SDL_GetPointDisplayIndex
#undef SDL_GetRectDisplayIndex
#undef SDL_GetRendererOutputSize
#undef SDL_GetTicks64
#undef SDL_GetWindowDisplayIndex
#undef SDL_GetWindowDisplayMode
#undef SDL_GL_DeleteContext
#undef SDL_GLattr
#undef SDL_GLcontextFlag
#undef SDL_GLcontextReleaseFlag
#undef SDL_GLprofile
#undef SDL_GUIDFromString
#undef SDL_HapticClose
#undef SDL_HapticDestroyEffect
#undef SDL_HapticGetEffectStatus
#undef SDL_HapticNewEffect
#undef SDL_HapticNumAxes
#undef SDL_HapticNumEffects
#undef SDL_HapticNumEffectsPlaying
#undef SDL_HapticOpen
#undef SDL_HapticOpenFromJoystick
#undef SDL_HapticOpenFromMouse
#undef SDL_HapticPause
#undef SDL_HapticQuery
#undef SDL_HapticRumbleInit
#undef SDL_HapticRumblePlay
#undef SDL_HapticRumbleStop
#undef SDL_HapticRunEffect
#undef SDL_HapticSetAutocenter
#undef SDL_HapticSetGain
#undef SDL_HapticStopAll
#undef SDL_HapticStopEffect
#undef SDL_HapticUnpause
#undef SDL_HapticUpdateEffect
#undef SDL_HasColorKey
#undef SDL_HasIntersection
#undef SDL_HasIntersectionF
#undef SDL_HasSurfaceRLE
#undef SDL_HasWindowSurface
#undef SDL_HINT_ALLOW_TOPMOST
#undef SDL_HINT_DIRECTINPUT_ENABLED
#undef SDL_HINT_GDK_TEXTINPUT_DEFAULT
#undef SDL_HINT_JOYSTICK_GAMECUBE_RUMBLE_BRAKE
#undef SDL_HINT_JOYSTICK_HIDAPI_PS4_RUMBLE
#undef SDL_HINT_JOYSTICK_HIDAPI_PS5_RUMBLE
#undef SDL_HINT_LINUX_DIGITAL_HATS
#undef SDL_HINT_LINUX_HAT_DEADZONES
#undef SDL_HINT_LINUX_JOYSTICK_CLASSIC
#undef SDL_HINT_LINUX_JOYSTICK_DEADZONES
#undef SDL_INIT_GAMECONTROLLER
#undef SDL_IntersectFRect
#undef SDL_IntersectFRectAndLine
#undef SDL_IntersectRect
#undef SDL_IntersectRectAndLine
#undef SDL_iOSSetAnimationCallback
#undef SDL_iOSSetEventPump
#undef SDL_iPhoneSetAnimationCallback
#undef SDL_iPhoneSetEventPump
#undef SDL_IsGameController
#undef SDL_IsScreenKeyboardShown
#undef SDL_IsScreenSaverEnabled
#undef SDL_IsTextInputActive
#undef SDL_JOYAXISMOTION
#undef SDL_JOYBALLMOTION
#undef SDL_JOYBATTERYUPDATED
#undef SDL_JOYBUTTONDOWN
#undef SDL_JOYBUTTONUP
#undef SDL_JOYDEVICEADDED
#undef SDL_JOYDEVICEREMOVED
#undef SDL_JOYHATMOTION
#undef SDL_JOYSTICK_TYPE_GAMECONTROLLER
#undef SDL_JoystickAttachVirtualEx
#undef SDL_JoystickClose
#undef SDL_JoystickDetachVirtual
#undef SDL_JoystickFromInstanceID
#undef SDL_JoystickFromPlayerIndex
#undef SDL_JoystickGetAttached
#undef SDL_JoystickGetAxis
#undef SDL_JoystickGetAxisInitialState
#undef SDL_JoystickGetBall
#undef SDL_JoystickGetButton
#undef SDL_JoystickGetFirmwareVersion
#undef SDL_JoystickGetGUID
#undef SDL_JoystickGetGUIDFromString
#undef SDL_JoystickGetHat
#undef SDL_JoystickGetPlayerIndex
#undef SDL_JoystickGetProduct
#undef SDL_JoystickGetProductVersion
#undef SDL_JoystickGetSerial
#undef SDL_JoystickGetType
#undef SDL_JoystickGetVendor
#undef SDL_JoystickGUID
#undef SDL_JoystickInstanceID
#undef SDL_JoystickIsHaptic
#undef SDL_JoystickIsVirtual
#undef SDL_JoystickName
#undef SDL_JoystickNumAxes
#undef SDL_JoystickNumBalls
#undef SDL_JoystickNumButtons
#undef SDL_JoystickNumHats
#undef SDL_JoystickOpen
#undef SDL_JoystickPath
#undef SDL_JoystickRumble
#undef SDL_JoystickRumbleTriggers
#undef SDL_JoystickSendEffect
#undef SDL_JoystickSetLED
#undef SDL_JoystickSetPlayerIndex
#undef SDL_JoystickSetVirtualAxis
#undef SDL_JoystickSetVirtualButton
#undef SDL_JoystickSetVirtualHat
#undef SDL_JoystickUpdate
#undef SDL_KEYDOWN
#undef SDL_KEYMAPCHANGED
#undef SDL_KEYUP
#undef SDL_LASTEVENT
#undef SDL_LinuxSetThreadPriority
#undef SDL_LinuxSetThreadPriorityAndPolicy
#undef SDL_LoadBMP_RW
#undef SDL_LoadWAV_RW
#undef SDL_LOCALECHANGED
#undef SDL_LogGetOutputFunction
#undef SDL_LogGetPriority
#undef SDL_LogResetPriorities
#undef SDL_LogSetAllPriority
#undef SDL_LogSetOutputFunction
#undef SDL_LogSetPriority
#undef SDL_LowerBlit
#undef SDL_LowerBlitScaled
#undef SDL_MasksToPixelFormatEnum
#undef SDL_MESSAGEBOX_COLOR_MAX
#undef SDL_MixAudioFormat
#undef SDL_MOUSEBUTTONDOWN
#undef SDL_MOUSEBUTTONUP
#undef SDL_MouseIsHaptic
#undef SDL_MOUSEMOTION
#undef SDL_MOUSEWHEEL
#undef SDL_mutex
#undef SDL_NewAudioStream
#undef SDL_NUM_LOG_PRIORITIES
#undef SDL_NUM_SCANCODES
#undef SDL_NUM_SYSTEM_CURSORS
#undef SDL_OnApplicationDidBecomeActive
#undef SDL_OnApplicationWillResignActive
#undef SDL_PATCHLEVEL
#undef SDL_PIXELFORMAT_BGR444
#undef SDL_PIXELFORMAT_BGR555
#undef SDL_PIXELFORMAT_BGR888
#undef SDL_PIXELFORMAT_RGB444
#undef SDL_PIXELFORMAT_RGB555
#undef SDL_PIXELFORMAT_RGB888
#undef SDL_PixelFormatEnumToMasks
#undef SDL_PointInFRect
#undef SDL_POLLSENTINEL
#undef SDL_PREALLOC
#undef SDL_QUIT
#undef SDL_ReadBE16
#undef SDL_ReadBE32
#undef SDL_ReadBE64
#undef SDL_ReadLE16
#undef SDL_ReadLE32
#undef SDL_ReadLE64
#undef SDL_RectEquals
#undef SDL_RENDER_DEVICE_RESET
#undef SDL_RENDER_TARGETS_RESET
#undef SDL_RenderCopy
#undef SDL_RenderCopyEx
#undef SDL_RenderCopyExF
#undef SDL_RenderCopyF
#undef SDL_RenderDrawLine
#undef SDL_RenderDrawLineF
#undef SDL_RenderDrawLines
#undef SDL_RenderDrawLinesF
#undef SDL_RenderDrawPoint
#undef SDL_RenderDrawPointF
#undef SDL_RenderDrawPoints
#undef SDL_RenderDrawPointsF
#undef SDL_RenderDrawRect
#undef SDL_RenderDrawRectF
#undef SDL_RenderDrawRects
#undef SDL_RenderDrawRectsF
#undef SDL_RendererFlip
#undef SDL_RenderFillRectF
#undef SDL_RenderFillRectsF
#undef SDL_RenderFlush
#undef SDL_RenderGetClipRect
#undef SDL_RenderGetLogicalSize
#undef SDL_RenderGetMetalCommandEncoder
#undef SDL_RenderGetMetalLayer
#undef SDL_RenderGetScale
#undef SDL_RenderGetViewport
#undef SDL_RenderGetWindow
#undef SDL_RenderIsClipEnabled
#undef SDL_RenderLogicalToWindow
#undef SDL_RenderSetClipRect
#undef SDL_RenderSetLogicalSize
#undef SDL_RenderSetScale
#undef SDL_RenderSetViewport
#undef SDL_RenderSetVSync
#undef SDL_RenderWindowToLogical
#undef SDL_RWclose
#undef SDL_RWFromConstMem
#undef SDL_RWFromFile
#undef SDL_RWFromMem
#undef SDL_RWops
#undef SDL_RWread
#undef SDL_RWseek
#undef SDL_RWsize
#undef SDL_RWtell
#undef SDL_RWwrite
#undef SDL_SaveBMP_RW
#undef SDL_ScaleModeLinear
#undef SDL_ScaleModeNearest
#undef SDL_SCANCODE_AUDIOFASTFORWARD
#undef SDL_SCANCODE_AUDIOMUTE
#undef SDL_SCANCODE_AUDIONEXT
#undef SDL_SCANCODE_AUDIOPLAY
#undef SDL_SCANCODE_AUDIOPREV
#undef SDL_SCANCODE_AUDIOREWIND
#undef SDL_SCANCODE_AUDIOSTOP
#undef SDL_SCANCODE_BRIGHTNESSDOWN
#undef SDL_SCANCODE_BRIGHTNESSUP
#undef SDL_SCANCODE_CALCULATOR
#undef SDL_SCANCODE_COMPUTER
#undef SDL_SCANCODE_DISPLAYSWITCH
#undef SDL_SCANCODE_KBDILLUMDOWN
#undef SDL_SCANCODE_KBDILLUMTOGGLE
#undef SDL_SCANCODE_KBDILLUMUP
#undef SDL_SCANCODE_MAIL
#undef SDL_SCANCODE_MEDIASELECT
#undef SDL_SCANCODE_WWW
#undef SDL_sem
#undef SDL_SemPost
#undef SDL_SemTryWait
#undef SDL_SemValue
#undef SDL_SemWait
#undef SDL_SemWaitTimeout
#undef SDL_SensorClose
#undef SDL_SensorFromInstanceID
#undef SDL_SensorGetData
#undef SDL_SensorGetInstanceID
#undef SDL_SensorGetName
#undef SDL_SensorGetNonPortableType
#undef SDL_SensorGetType
#undef SDL_SensorOpen
#undef SDL_SensorUpdate
#undef SDL_SENSORUPDATE
#undef SDL_SetClipRect
#undef SDL_SetColorKey
#undef SDL_SetThreadPriority
#undef SDL_SetWindowDisplayMode
#undef SDL_SIMD_ALIGNED
#undef SDL_SIMDGetAlignment
#undef SDL_size_add_overflow
#undef SDL_size_mul_overflow
#undef SDL_strtokr
#undef SDL_SwapBE16
#undef SDL_SwapBE32
#undef SDL_SwapBE64
#undef SDL_SwapLE16
#undef SDL_SwapLE32
#undef SDL_SwapLE64
#undef SDL_SYSTEM_CURSOR_ARROW
#undef SDL_SYSTEM_CURSOR_HAND
#undef SDL_SYSTEM_CURSOR_IBEAM
#undef SDL_SYSTEM_CURSOR_NO
#undef SDL_SYSTEM_CURSOR_SIZEALL
#undef SDL_SYSTEM_CURSOR_SIZENESW
#undef SDL_SYSTEM_CURSOR_SIZENS
#undef SDL_SYSTEM_CURSOR_SIZENWSE
#undef SDL_SYSTEM_CURSOR_SIZEWE
#undef SDL_SYSTEM_CURSOR_WAITARROW
#undef SDL_SYSTEM_CURSOR_WINDOW_BOTTOM
#undef SDL_SYSTEM_CURSOR_WINDOW_BOTTOMLEFT
#undef SDL_SYSTEM_CURSOR_WINDOW_BOTTOMRIGHT
#undef SDL_SYSTEM_CURSOR_WINDOW_LEFT
#undef SDL_SYSTEM_CURSOR_WINDOW_RIGHT
#undef SDL_SYSTEM_CURSOR_WINDOW_TOP
#undef SDL_SYSTEM_CURSOR_WINDOW_TOPLEFT
#undef SDL_SYSTEM_CURSOR_WINDOW_TOPRIGHT
#undef SDL_TABLESIZE
#undef SDL_TEXTEDITING
#undef SDL_TEXTEDITING_EXT
#undef SDL_TEXTINPUT
#undef SDL_threadID
#undef SDL_TLSCleanup
#undef SDL_TLSGet
#undef SDL_TLSSet
#undef SDL_TRUE
#undef SDL_UnionFRect
#undef SDL_UnionRect
#undef SDL_UpperBlit
#undef SDL_UpperBlitScaled
#undef SDL_USEREVENT
#undef SDL_WINDOW_ALLOW_HIGHDPI
#undef SDL_WINDOW_FULLSCREEN_DESKTOP
#undef SDL_WINDOW_INPUT_GRABBED
#undef SDL_WINDOW_SKIP_TASKBAR
#undef SDL_WINDOWEVENT_CLOSE
#undef SDL_WINDOWEVENT_DISPLAY_CHANGED
#undef SDL_WINDOWEVENT_ENTER
#undef SDL_WINDOWEVENT_EXPOSED
#undef SDL_WINDOWEVENT_FOCUS_GAINED
#undef SDL_WINDOWEVENT_FOCUS_LOST
#undef SDL_WINDOWEVENT_HIDDEN
#undef SDL_WINDOWEVENT_HIT_TEST
#undef SDL_WINDOWEVENT_ICCPROF_CHANGED
#undef SDL_WINDOWEVENT_LEAVE
#undef SDL_WINDOWEVENT_MAXIMIZED
#undef SDL_WINDOWEVENT_MINIMIZED
#undef SDL_WINDOWEVENT_MOVED
#undef SDL_WINDOWEVENT_RESIZED
#undef SDL_WINDOWEVENT_RESTORED
#undef SDL_WINDOWEVENT_SHOWN
#undef SDL_WINDOWEVENT_SIZE_CHANGED
#undef SDL_WriteBE16
#undef SDL_WriteBE32
#undef SDL_WriteBE64
#undef SDL_WriteLE16
#undef SDL_WriteLE32
#undef SDL_WriteLE64
#undef SDLK_a
#undef SDLK_AUDIOFASTFORWARD
#undef SDLK_AUDIOMUTE
#undef SDLK_AUDIONEXT
#undef SDLK_AUDIOPLAY
#undef SDLK_AUDIOPREV
#undef SDLK_AUDIOREWIND
#undef SDLK_AUDIOSTOP
#undef SDLK_b
#undef SDLK_BACKQUOTE
#undef SDLK_c
#undef SDLK_d
#undef SDLK_e
#undef SDLK_EJECT
#undef SDLK_f
#undef SDLK_g
#undef SDLK_h
#undef SDLK_i
#undef SDLK_j
#undef SDLK_k
#undef SDLK_l
#undef SDLK_m
#undef SDLK_MEDIASELECT
#undef SDLK_n
#undef SDLK_o
#undef SDLK_p
#undef SDLK_q
#undef SDLK_QUOTE
#undef SDLK_QUOTEDBL
#undef SDLK_r
#undef SDLK_s
#undef SDLK_t
#undef SDLK_u
#undef SDLK_v
#undef SDLK_w
#undef SDLK_x
#undef SDLK_y
#undef SDLK_z

#define AUDIO_F32                                        SDL_AUDIO_F32LE
#define AUDIO_F32LSB                                     SDL_AUDIO_F32LE
#define AUDIO_F32MSB                                     SDL_AUDIO_F32BE
#define AUDIO_F32SYS                                     SDL_AUDIO_F32
#define AUDIO_S16                                        SDL_AUDIO_S16LE
#define AUDIO_S16LSB                                     SDL_AUDIO_S16LE
#define AUDIO_S16MSB                                     SDL_AUDIO_S16BE
#define AUDIO_S16SYS                                     SDL_AUDIO_S16
#define AUDIO_S32                                        SDL_AUDIO_S32LE
#define AUDIO_S32LSB                                     SDL_AUDIO_S32LE
#define AUDIO_S32MSB                                     SDL_AUDIO_S32BE
#define AUDIO_S32SYS                                     SDL_AUDIO_S32
#define AUDIO_S8                                         SDL_AUDIO_S8
#define AUDIO_U8                                         SDL_AUDIO_U8
#define KMOD_ALT                                         SDL_KMOD_ALT
#define KMOD_CAPS                                        SDL_KMOD_CAPS
#define KMOD_CTRL                                        SDL_KMOD_CTRL
#define KMOD_GUI                                         SDL_KMOD_GUI
#define KMOD_LALT                                        SDL_KMOD_LALT
#define KMOD_LCTRL                                       SDL_KMOD_LCTRL
#define KMOD_LGUI                                        SDL_KMOD_LGUI
#define KMOD_LSHIFT                                      SDL_KMOD_LSHIFT
#define KMOD_MODE                                        SDL_KMOD_MODE
#define KMOD_NONE                                        SDL_KMOD_NONE
#define KMOD_NUM                                         SDL_KMOD_NUM
#define KMOD_RALT                                        SDL_KMOD_RALT
#define KMOD_RCTRL                                       SDL_KMOD_RCTRL
#define KMOD_RGUI                                        SDL_KMOD_RGUI
#define KMOD_RSHIFT                                      SDL_KMOD_RSHIFT
#define KMOD_SCROLL                                      SDL_KMOD_SCROLL
#define KMOD_SHIFT                                       SDL_KMOD_SHIFT
#define RW_SEEK_CUR                                      SDL_IO_SEEK_CUR
#define RW_SEEK_END                                      SDL_IO_SEEK_END
#define RW_SEEK_SET                                      SDL_IO_SEEK_SET
#define SDL_AllocFormat                                  SDL_GetPixelFormatDetails
#define SDL_AllocPalette                                 SDL_CreatePalette
#define SDL_AndroidBackButton                            SDL_SendAndroidBackButton
#define SDL_AndroidGetActivity                           SDL_GetAndroidActivity
#define SDL_AndroidGetExternalStoragePath                SDL_GetAndroidExternalStoragePath
#define SDL_AndroidGetExternalStorageState               SDL_GetAndroidExternalStorageState
#define SDL_AndroidGetInternalStoragePath                SDL_GetAndroidInternalStoragePath
#define SDL_AndroidGetJNIEnv                             SDL_GetAndroidJNIEnv
#define SDL_AndroidRequestPermission                     SDL_RequestAndroidPermission
#define SDL_AndroidRequestPermissionCallback             SDL_RequestAndroidPermissionCallback
#define SDL_AndroidSendMessage                           SDL_SendAndroidMessage
#define SDL_AndroidShowToast                             SDL_ShowAndroidToast
#define SDL_APP_DIDENTERBACKGROUND                       SDL_EVENT_DID_ENTER_BACKGROUND
#define SDL_APP_DIDENTERFOREGROUND                       SDL_EVENT_DID_ENTER_FOREGROUND
#define SDL_APP_LOWMEMORY                                SDL_EVENT_LOW_MEMORY
#define SDL_APP_TERMINATING                              SDL_EVENT_TERMINATING
#define SDL_APP_WILLENTERBACKGROUND                      SDL_EVENT_WILL_ENTER_BACKGROUND
#define SDL_APP_WILLENTERFOREGROUND                      SDL_EVENT_WILL_ENTER_FOREGROUND
#define SDL_atomic_t                                     SDL_AtomicInt
#define SDL_AtomicAdd                                    SDL_AddAtomicInt
#define SDL_AtomicCAS                                    SDL_CompareAndSwapAtomicInt
#define SDL_AtomicCASPtr                                 SDL_CompareAndSwapAtomicPointer
#define SDL_AtomicGet                                    SDL_GetAtomicInt
#define SDL_AtomicGetPtr                                 SDL_GetAtomicPointer
#define SDL_AtomicLock                                   SDL_LockSpinlock
#define SDL_AtomicSet                                    SDL_SetAtomicInt
#define SDL_AtomicSetPtr                                 SDL_SetAtomicPointer
#define SDL_AtomicTryLock                                SDL_TryLockSpinlock
#define SDL_AtomicUnlock                                 SDL_UnlockSpinlock
#define SDL_AUDIODEVICEADDED                             SDL_EVENT_AUDIO_DEVICE_ADDED
#define SDL_AUDIODEVICEREMOVED                           SDL_EVENT_AUDIO_DEVICE_REMOVED
#define SDL_AudioStreamAvailable                         SDL_GetAudioStreamAvailable
#define SDL_AudioStreamClear                             SDL_ClearAudioStream
#define SDL_AudioStreamFlush                             SDL_FlushAudioStream
#define SDL_AudioStreamGet                               SDL_GetAudioStreamData
#define SDL_AudioStreamPut                               SDL_PutAudioStreamData
#define SDL_BlitScaled                                   SDL_BlitSurfaceScaled
#define SDL_bool                                         bool
#define SDL_BUTTON(_x_)                                  SDL_BUTTON_MASK(_x_)
#define SDL_CLIPBOARDUPDATE                              SDL_EVENT_CLIPBOARD_UPDATE
#define SDL_Colour                                       SDL_Color
#define SDL_COMPILEDVERSION                              SDL_VERSION
#define SDL_cond                                         SDL_Condition
#define SDL_CondBroadcast                                SDL_BroadcastCondition
#define SDL_CondSignal                                   SDL_SignalCondition
#define SDL_CondWait                                     SDL_WaitCondition
#define SDL_CondWaitTimeout                              SDL_WaitConditionTimeout
#define SDL_CONTROLLER_AXIS_INVALID                      SDL_GAMEPAD_AXIS_INVALID
#define SDL_CONTROLLER_AXIS_LEFTX                        SDL_GAMEPAD_AXIS_LEFTX
#define SDL_CONTROLLER_AXIS_LEFTY                        SDL_GAMEPAD_AXIS_LEFTY
#define SDL_CONTROLLER_AXIS_MAX                          SDL_GAMEPAD_AXIS_COUNT
#define SDL_CONTROLLER_AXIS_RIGHTX                       SDL_GAMEPAD_AXIS_RIGHTX
#define SDL_CONTROLLER_AXIS_RIGHTY                       SDL_GAMEPAD_AXIS_RIGHTY
#define SDL_CONTROLLER_AXIS_TRIGGERLEFT                  SDL_GAMEPAD_AXIS_LEFT_TRIGGER
#define SDL_CONTROLLER_AXIS_TRIGGERRIGHT                 SDL_GAMEPAD_AXIS_RIGHT_TRIGGER
#define SDL_CONTROLLER_BINDTYPE_AXIS                     SDL_GAMEPAD_BINDTYPE_AXIS
#define SDL_CONTROLLER_BINDTYPE_BUTTON                   SDL_GAMEPAD_BINDTYPE_BUTTON
#define SDL_CONTROLLER_BINDTYPE_HAT                      SDL_GAMEPAD_BINDTYPE_HAT
#define SDL_CONTROLLER_BINDTYPE_NONE                     SDL_GAMEPAD_BINDTYPE_NONE
#define SDL_CONTROLLER_BUTTON_A                          SDL_GAMEPAD_BUTTON_SOUTH
#define SDL_CONTROLLER_BUTTON_B                          SDL_GAMEPAD_BUTTON_EAST
#define SDL_CONTROLLER_BUTTON_BACK                       SDL_GAMEPAD_BUTTON_BACK
#define SDL_CONTROLLER_BUTTON_DPAD_DOWN                  SDL_GAMEPAD_BUTTON_DPAD_DOWN
#define SDL_CONTROLLER_BUTTON_DPAD_LEFT                  SDL_GAMEPAD_BUTTON_DPAD_LEFT
#define SDL_CONTROLLER_BUTTON_DPAD_RIGHT                 SDL_GAMEPAD_BUTTON_DPAD_RIGHT
#define SDL_CONTROLLER_BUTTON_DPAD_UP                    SDL_GAMEPAD_BUTTON_DPAD_UP
#define SDL_CONTROLLER_BUTTON_GUIDE                      SDL_GAMEPAD_BUTTON_GUIDE
#define SDL_CONTROLLER_BUTTON_INVALID                    SDL_GAMEPAD_BUTTON_INVALID
#define SDL_CONTROLLER_BUTTON_LEFTSHOULDER               SDL_GAMEPAD_BUTTON_LEFT_SHOULDER
#define SDL_CONTROLLER_BUTTON_LEFTSTICK                  SDL_GAMEPAD_BUTTON_LEFT_STICK
#define SDL_CONTROLLER_BUTTON_MAX                        SDL_GAMEPAD_BUTTON_COUNT
#define SDL_CONTROLLER_BUTTON_MISC1                      SDL_GAMEPAD_BUTTON_MISC1
#define SDL_CONTROLLER_BUTTON_PADDLE1                    SDL_GAMEPAD_BUTTON_RIGHT_PADDLE1
#define SDL_CONTROLLER_BUTTON_PADDLE2                    SDL_GAMEPAD_BUTTON_LEFT_PADDLE1
#define SDL_CONTROLLER_BUTTON_PADDLE3                    SDL_GAMEPAD_BUTTON_RIGHT_PADDLE2
#define SDL_CONTROLLER_BUTTON_PADDLE4                    SDL_GAMEPAD_BUTTON_LEFT_PADDLE2
#define SDL_CONTROLLER_BUTTON_RIGHTSHOULDER              SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER
#define SDL_CONTROLLER_BUTTON_RIGHTSTICK                 SDL_GAMEPAD_BUTTON_RIGHT_STICK
#define SDL_CONTROLLER_BUTTON_START                      SDL_GAMEPAD_BUTTON_START
#define SDL_CONTROLLER_BUTTON_TOUCHPAD                   SDL_GAMEPAD_BUTTON_TOUCHPAD
#define SDL_CONTROLLER_BUTTON_X                          SDL_GAMEPAD_BUTTON_WEST
#define SDL_CONTROLLER_BUTTON_Y                          SDL_GAMEPAD_BUTTON_NORTH
#define SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT  SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_LEFT
#define SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR  SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_PAIR
#define SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT
#define SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO          SDL_GAMEPAD_TYPE_NINTENDO_SWITCH_PRO
#define SDL_CONTROLLER_TYPE_PS3                          SDL_GAMEPAD_TYPE_PS3
#define SDL_CONTROLLER_TYPE_PS4                          SDL_GAMEPAD_TYPE_PS4
#define SDL_CONTROLLER_TYPE_PS5                          SDL_GAMEPAD_TYPE_PS5
#define SDL_CONTROLLER_TYPE_UNKNOWN                      SDL_GAMEPAD_TYPE_STANDARD
#define SDL_CONTROLLER_TYPE_VIRTUAL                      SDL_GAMEPAD_TYPE_VIRTUAL
#define SDL_CONTROLLER_TYPE_XBOX360                      SDL_GAMEPAD_TYPE_XBOX360
#define SDL_CONTROLLER_TYPE_XBOXONE                      SDL_GAMEPAD_TYPE_XBOXONE
#define SDL_ControllerAxisEvent                          SDL_GamepadAxisEvent
#define SDL_CONTROLLERAXISMOTION                         SDL_EVENT_GAMEPAD_AXIS_MOTION
#define SDL_CONTROLLERBUTTONDOWN                         SDL_EVENT_GAMEPAD_BUTTON_DOWN
#define SDL_ControllerButtonEvent                        SDL_GamepadButtonEvent
#define SDL_CONTROLLERBUTTONUP                           SDL_EVENT_GAMEPAD_BUTTON_UP
#define SDL_CONTROLLERDEVICEADDED                        SDL_EVENT_GAMEPAD_ADDED
#define SDL_ControllerDeviceEvent                        SDL_GamepadDeviceEvent
#define SDL_CONTROLLERDEVICEREMAPPED                     SDL_EVENT_GAMEPAD_REMAPPED
#define SDL_CONTROLLERDEVICEREMOVED                      SDL_EVENT_GAMEPAD_REMOVED
#define SDL_ControllerSensorEvent                        SDL_GamepadSensorEvent
#define SDL_CONTROLLERSENSORUPDATE                       SDL_EVENT_GAMEPAD_SENSOR_UPDATE
#define SDL_CONTROLLERSTEAMHANDLEUPDATED                 SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED
#define SDL_CONTROLLERTOUCHPADDOWN                       SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN
#define SDL_ControllerTouchpadEvent                      SDL_GamepadTouchpadEvent
#define SDL_CONTROLLERTOUCHPADMOTION                     SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION
#define SDL_CONTROLLERTOUCHPADUP                         SDL_EVENT_GAMEPAD_TOUCHPAD_UP
#define SDL_ConvertSurfaceFormat                         SDL_ConvertSurface
#define SDL_CreateCond                                   SDL_CreateCondition
#define SDL_DelEventWatch                                SDL_RemoveEventWatch
#define SDL_DelHintCallback                              SDL_RemoveHintCallback
#define SDL_DestroyCond                                  SDL_DestroyCondition
#define SDL_Direct3D9GetAdapterIndex                     SDL_GetDirect3D9AdapterIndex
#define SDL_DISPLAYEVENT_CONNECTED                       SDL_EVENT_DISPLAY_ADDED
#define SDL_DISPLAYEVENT_DISCONNECTED                    SDL_EVENT_DISPLAY_REMOVED
#define SDL_DISPLAYEVENT_MOVED                           SDL_EVENT_DISPLAY_MOVED
#define SDL_DISPLAYEVENT_ORIENTATION                     SDL_EVENT_DISPLAY_ORIENTATION
#define SDL_DROPBEGIN                                    SDL_EVENT_DROP_BEGIN
#define SDL_DROPCOMPLETE                                 SDL_EVENT_DROP_COMPLETE
#define SDL_DROPFILE                                     SDL_EVENT_DROP_FILE
#define SDL_DROPTEXT                                     SDL_EVENT_DROP_TEXT
#define SDL_DXGIGetOutputInfo                            SDL_GetDXGIOutputInfo
#define SDL_EncloseFPoints                               SDL_GetRectEnclosingPointsFloat
#define SDL_EnclosePoints                                SDL_GetRectEnclosingPoints
#define SDL_eventaction                                  SDL_EventAction
#define SDL_FALSE                                        false
#define SDL_FillRect                                     SDL_FillSurfaceRect
#define SDL_FillRects                                    SDL_FillSurfaceRects
#define SDL_FINGERDOWN                                   SDL_EVENT_FINGER_DOWN
#define SDL_FINGERMOTION                                 SDL_EVENT_FINGER_MOTION
#define SDL_FINGERUP                                     SDL_EVENT_FINGER_UP
#define SDL_FIRSTEVENT                                   SDL_EVENT_FIRST
#define SDL_FRectEmpty                                   SDL_RectEmptyFloat
#define SDL_FRectEquals                                  SDL_RectsEqualFloat
#define SDL_FRectEqualsEpsilon                           SDL_RectsEqualEpsilon
#define SDL_FreeAudioStream                              SDL_DestroyAudioStream
#define SDL_FreeCursor                                   SDL_DestroyCursor
#define SDL_FreePalette                                  SDL_DestroyPalette
#define SDL_FreeSurface                                  SDL_DestroySurface
#define SDL_FreeWAV                                      SDL_free
#define SDL_GameController                               SDL_Gamepad
#define SDL_GameControllerAddMapping                     SDL_AddGamepadMapping
#define SDL_GameControllerAddMappingsFromFile            SDL_AddGamepadMappingsFromFile
#define SDL_GameControllerAddMappingsFromRW              SDL_AddGamepadMappingsFromIO
#define SDL_GameControllerAxis                           SDL_GamepadAxis
#define SDL_GameControllerBindType                       SDL_GamepadBindingType
#define SDL_GameControllerButton                         SDL_GamepadButton
#define SDL_GameControllerClose                          SDL_CloseGamepad
#define SDL_GameControllerFromInstanceID                 SDL_GetGamepadFromID
#define SDL_GameControllerFromPlayerIndex                SDL_GetGamepadFromPlayerIndex
#define SDL_GameControllerGetAppleSFSymbolsNameForAxis   SDL_GetGamepadAppleSFSymbolsNameForAxis
#define SDL_GameControllerGetAppleSFSymbolsNameForButton SDL_GetGamepadAppleSFSymbolsNameForButton
#define SDL_GameControllerGetAttached                    SDL_GamepadConnected
#define SDL_GameControllerGetAxis                        SDL_GetGamepadAxis
#define SDL_GameControllerGetAxisFromString              SDL_GetGamepadAxisFromString
#define SDL_GameControllerGetButton                      SDL_GetGamepadButton
#define SDL_GameControllerGetButtonFromString            SDL_GetGamepadButtonFromString
#define SDL_GameControllerGetFirmwareVersion             SDL_GetGamepadFirmwareVersion
#define SDL_GameControllerGetJoystick                    SDL_GetGamepadJoystick
#define SDL_GameControllerGetNumTouchpadFingers          SDL_GetNumGamepadTouchpadFingers
#define SDL_GameControllerGetNumTouchpads                SDL_GetNumGamepadTouchpads
#define SDL_GameControllerGetPlayerIndex                 SDL_GetGamepadPlayerIndex
#define SDL_GameControllerGetProduct                     SDL_GetGamepadProduct
#define SDL_GameControllerGetProductVersion              SDL_GetGamepadProductVersion
#define SDL_GameControllerGetSensorData                  SDL_GetGamepadSensorData
#define SDL_GameControllerGetSensorDataRate              SDL_GetGamepadSensorDataRate
#define SDL_GameControllerGetSerial                      SDL_GetGamepadSerial
#define SDL_GameControllerGetSteamHandle                 SDL_GetGamepadSteamHandle
#define SDL_GameControllerGetStringForAxis               SDL_GetGamepadStringForAxis
#define SDL_GameControllerGetStringForButton             SDL_GetGamepadStringForButton
#define SDL_GameControllerGetTouchpadFinger              SDL_GetGamepadTouchpadFinger
#define SDL_GameControllerGetType                        SDL_GetGamepadType
#define SDL_GameControllerGetVendor                      SDL_GetGamepadVendor
#define SDL_GameControllerHasAxis                        SDL_GamepadHasAxis
#define SDL_GameControllerHasButton                      SDL_GamepadHasButton
#define SDL_GameControllerHasSensor                      SDL_GamepadHasSensor
#define SDL_GameControllerIsSensorEnabled                SDL_GamepadSensorEnabled
#define SDL_GameControllerMapping                        SDL_GetGamepadMapping
#define SDL_GameControllerMappingForDeviceIndex          SDL_GetGamepadMappingForDeviceIndex
#define SDL_GameControllerMappingForGUID                 SDL_GetGamepadMappingForGUID
#define SDL_GameControllerName                           SDL_GetGamepadName
#define SDL_GameControllerOpen                           SDL_OpenGamepad
#define SDL_GameControllerPath                           SDL_GetGamepadPath
#define SDL_GameControllerRumble                         SDL_RumbleGamepad
#define SDL_GameControllerRumbleTriggers                 SDL_RumbleGamepadTriggers
#define SDL_GameControllerSendEffect                     SDL_SendGamepadEffect
#define SDL_GameControllerSetLED                         SDL_SetGamepadLED
#define SDL_GameControllerSetPlayerIndex                 SDL_SetGamepadPlayerIndex
#define SDL_GameControllerSetSensorEnabled               SDL_SetGamepadSensorEnabled
#define SDL_GameControllerType                           SDL_GamepadType
#define SDL_GameControllerUpdate                         SDL_UpdateGamepads
#define SDL_GDKGetDefaultUser                            SDL_GetGDKDefaultUser
#define SDL_GDKGetTaskQueue                              SDL_GetGDKTaskQueue
#define SDL_GetClipRect                                  SDL_GetSurfaceClipRect
#define SDL_GetClosestDisplayMode                        SDL_GetClosestFullscreenDisplayMode
#define SDL_GetColorKey                                  SDL_GetSurfaceColorKey
#define SDL_GetCPUCount                                  SDL_GetNumLogicalCPUCores
#define SDL_GetDisplayOrientation                        SDL_GetCurrentDisplayOrientation
#define SDL_GetPointDisplayIndex                         SDL_GetDisplayForPoint
#define SDL_GetRectDisplayIndex                          SDL_GetDisplayForRect
#define SDL_GetRendererOutputSize                        SDL_GetCurrentRenderOutputSize
#define SDL_GetTicks64                                   SDL_GetTicks
#define SDL_GetWindowDisplayIndex                        SDL_GetDisplayForWindow
#define SDL_GetWindowDisplayMode                         SDL_GetWindowFullscreenMode
#define SDL_GL_DeleteContext                             SDL_GL_DestroyContext
#define SDL_GLattr                                       SDL_GLAttr
#define SDL_GLcontextFlag                                SDL_GLContextFlag
#define SDL_GLcontextReleaseFlag                         SDL_GLContextReleaseFlag
#define SDL_GLprofile                                    SDL_GLProfile
#define SDL_GUIDFromString                               SDL_StringToGUID
#define SDL_HapticClose                                  SDL_CloseHaptic
#define SDL_HapticDestroyEffect                          SDL_DestroyHapticEffect
#define SDL_HapticGetEffectStatus                        SDL_GetHapticEffectStatus
#define SDL_HapticNewEffect                              SDL_CreateHapticEffect
#define SDL_HapticNumAxes                                SDL_GetNumHapticAxes
#define SDL_HapticNumEffects                             SDL_GetMaxHapticEffects
#define SDL_HapticNumEffectsPlaying                      SDL_GetMaxHapticEffectsPlaying
#define SDL_HapticOpen                                   SDL_OpenHaptic
#define SDL_HapticOpenFromJoystick                       SDL_OpenHapticFromJoystick
#define SDL_HapticOpenFromMouse                          SDL_OpenHapticFromMouse
#define SDL_HapticPause                                  SDL_PauseHaptic
#define SDL_HapticQuery                                  SDL_GetHapticFeatures
#define SDL_HapticRumbleInit                             SDL_InitHapticRumble
#define SDL_HapticRumblePlay                             SDL_PlayHapticRumble
#define SDL_HapticRumbleStop                             SDL_StopHapticRumble
#define SDL_HapticRunEffect                              SDL_RunHapticEffect
#define SDL_HapticSetAutocenter                          SDL_SetHapticAutocenter
#define SDL_HapticSetGain                                SDL_SetHapticGain
#define SDL_HapticStopAll                                SDL_StopHapticEffects
#define SDL_HapticStopEffect                             SDL_StopHapticEffect
#define SDL_HapticUnpause                                SDL_ResumeHaptic
#define SDL_HapticUpdateEffect                           SDL_UpdateHapticEffect
#define SDL_HasColorKey                                  SDL_SurfaceHasColorKey
#define SDL_HasIntersection                              SDL_HasRectIntersection
#define SDL_HasIntersectionF                             SDL_HasRectIntersectionFloat
#define SDL_HasSurfaceRLE                                SDL_SurfaceHasRLE
#define SDL_HasWindowSurface                             SDL_WindowHasSurface
#define SDL_HINT_ALLOW_TOPMOST                           SDL_HINT_WINDOW_ALLOW_TOPMOST
#define SDL_HINT_DIRECTINPUT_ENABLED                     SDL_HINT_JOYSTICK_DIRECTINPUT
#define SDL_HINT_GDK_TEXTINPUT_DEFAULT                   SDL_HINT_GDK_TEXTINPUT_DEFAULT_TEXT
#define SDL_HINT_JOYSTICK_GAMECUBE_RUMBLE_BRAKE          SDL_HINT_JOYSTICK_HIDAPI_GAMECUBE_RUMBLE_BRAKE
#define SDL_HINT_JOYSTICK_HIDAPI_PS4_RUMBLE              SDL_HINT_JOYSTICK_ENHANCED_REPORTS
#define SDL_HINT_JOYSTICK_HIDAPI_PS5_RUMBLE              SDL_HINT_JOYSTICK_ENHANCED_REPORTS
#define SDL_HINT_LINUX_DIGITAL_HATS                      SDL_HINT_JOYSTICK_LINUX_DIGITAL_HATS
#define SDL_HINT_LINUX_HAT_DEADZONES                     SDL_HINT_JOYSTICK_LINUX_HAT_DEADZONES
#define SDL_HINT_LINUX_JOYSTICK_CLASSIC                  SDL_HINT_JOYSTICK_LINUX_CLASSIC
#define SDL_HINT_LINUX_JOYSTICK_DEADZONES                SDL_HINT_JOYSTICK_LINUX_DEADZONES
#define SDL_INIT_GAMECONTROLLER                          SDL_INIT_GAMEPAD
#define SDL_IntersectFRect                               SDL_GetRectIntersectionFloat
#define SDL_IntersectFRectAndLine                        SDL_GetRectAndLineIntersectionFloat
#define SDL_IntersectRect                                SDL_GetRectIntersection
#define SDL_IntersectRectAndLine                         SDL_GetRectAndLineIntersection
#define SDL_iOSSetAnimationCallback                      SDL_SetiOSAnimationCallback
#define SDL_iOSSetEventPump                              SDL_SetiOSEventPump
#define SDL_iPhoneSetAnimationCallback                   SDL_iOSSetAnimationCallback
#define SDL_iPhoneSetEventPump                           SDL_iOSSetEventPump
#define SDL_IsGameController                             SDL_IsGamepad
#define SDL_IsScreenKeyboardShown                        SDL_ScreenKeyboardShown
#define SDL_IsScreenSaverEnabled                         SDL_ScreenSaverEnabled
#define SDL_IsTextInputActive                            SDL_TextInputActive
#define SDL_JOYAXISMOTION                                SDL_EVENT_JOYSTICK_AXIS_MOTION
#define SDL_JOYBALLMOTION                                SDL_EVENT_JOYSTICK_BALL_MOTION
#define SDL_JOYBATTERYUPDATED                            SDL_EVENT_JOYSTICK_BATTERY_UPDATED
#define SDL_JOYBUTTONDOWN                                SDL_EVENT_JOYSTICK_BUTTON_DOWN
#define SDL_JOYBUTTONUP                                  SDL_EVENT_JOYSTICK_BUTTON_UP
#define SDL_JOYDEVICEADDED                               SDL_EVENT_JOYSTICK_ADDED
#define SDL_JOYDEVICEREMOVED                             SDL_EVENT_JOYSTICK_REMOVED
#define SDL_JOYHATMOTION                                 SDL_EVENT_JOYSTICK_HAT_MOTION
#define SDL_JOYSTICK_TYPE_GAMECONTROLLER                 SDL_JOYSTICK_TYPE_GAMEPAD
#define SDL_JoystickAttachVirtualEx                      SDL_AttachVirtualJoystick
#define SDL_JoystickClose                                SDL_CloseJoystick
#define SDL_JoystickDetachVirtual                        SDL_DetachVirtualJoystick
#define SDL_JoystickFromInstanceID                       SDL_GetJoystickFromID
#define SDL_JoystickFromPlayerIndex                      SDL_GetJoystickFromPlayerIndex
#define SDL_JoystickGetAttached                          SDL_JoystickConnected
#define SDL_JoystickGetAxis                              SDL_GetJoystickAxis
#define SDL_JoystickGetAxisInitialState                  SDL_GetJoystickAxisInitialState
#define SDL_JoystickGetBall                              SDL_GetJoystickBall
#define SDL_JoystickGetButton                            SDL_GetJoystickButton
#define SDL_JoystickGetFirmwareVersion                   SDL_GetJoystickFirmwareVersion
#define SDL_JoystickGetGUID                              SDL_GetJoystickGUID
#define SDL_JoystickGetGUIDFromString                    SDL_GUIDFromString
#define SDL_JoystickGetHat                               SDL_GetJoystickHat
#define SDL_JoystickGetPlayerIndex                       SDL_GetJoystickPlayerIndex
#define SDL_JoystickGetProduct                           SDL_GetJoystickProduct
#define SDL_JoystickGetProductVersion                    SDL_GetJoystickProductVersion
#define SDL_JoystickGetSerial                            SDL_GetJoystickSerial
#define SDL_JoystickGetType                              SDL_GetJoystickType
#define SDL_JoystickGetVendor                            SDL_GetJoystickVendor
#define SDL_JoystickGUID                                 SDL_GUID
#define SDL_JoystickInstanceID                           SDL_GetJoystickID
#define SDL_JoystickIsHaptic                             SDL_IsJoystickHaptic
#define SDL_JoystickIsVirtual                            SDL_IsJoystickVirtual
#define SDL_JoystickName                                 SDL_GetJoystickName
#define SDL_JoystickNumAxes                              SDL_GetNumJoystickAxes
#define SDL_JoystickNumBalls                             SDL_GetNumJoystickBalls
#define SDL_JoystickNumButtons                           SDL_GetNumJoystickButtons
#define SDL_JoystickNumHats                              SDL_GetNumJoystickHats
#define SDL_JoystickOpen                                 SDL_OpenJoystick
#define SDL_JoystickPath                                 SDL_GetJoystickPath
#define SDL_JoystickRumble                               SDL_RumbleJoystick
#define SDL_JoystickRumbleTriggers                       SDL_RumbleJoystickTriggers
#define SDL_JoystickSendEffect                           SDL_SendJoystickEffect
#define SDL_JoystickSetLED                               SDL_SetJoystickLED
#define SDL_JoystickSetPlayerIndex                       SDL_SetJoystickPlayerIndex
#define SDL_JoystickSetVirtualAxis                       SDL_SetJoystickVirtualAxis
#define SDL_JoystickSetVirtualButton                     SDL_SetJoystickVirtualButton
#define SDL_JoystickSetVirtualHat                        SDL_SetJoystickVirtualHat
#define SDL_JoystickUpdate                               SDL_UpdateJoysticks
#define SDL_KEYDOWN                                      SDL_EVENT_KEY_DOWN
#define SDL_KEYMAPCHANGED                                SDL_EVENT_KEYMAP_CHANGED
#define SDL_KEYUP                                        SDL_EVENT_KEY_UP
#define SDL_LASTEVENT                                    SDL_EVENT_LAST
#define SDL_LinuxSetThreadPriority                       SDL_SetLinuxThreadPriority
#define SDL_LinuxSetThreadPriorityAndPolicy              SDL_SetLinuxThreadPriorityAndPolicy
#define SDL_LoadBMP_RW                                   SDL_LoadBMP_IO
#define SDL_LoadWAV_RW                                   SDL_LoadWAV_IO
#define SDL_LOCALECHANGED                                SDL_EVENT_LOCALE_CHANGED
#define SDL_LogGetOutputFunction                         SDL_GetLogOutputFunction
#define SDL_LogGetPriority                               SDL_GetLogPriority
#define SDL_LogResetPriorities                           SDL_ResetLogPriorities
#define SDL_LogSetAllPriority                            SDL_SetLogPriorities
#define SDL_LogSetOutputFunction                         SDL_SetLogOutputFunction
#define SDL_LogSetPriority                               SDL_SetLogPriority
#define SDL_LowerBlit                                    SDL_BlitSurfaceUnchecked
#define SDL_LowerBlitScaled                              SDL_BlitSurfaceUncheckedScaled
#define SDL_MasksToPixelFormatEnum                       SDL_GetPixelFormatForMasks
#define SDL_MESSAGEBOX_COLOR_MAX                         SDL_MESSAGEBOX_COLOR_COUNT
#define SDL_MixAudioFormat                               SDL_MixAudio
#define SDL_MOUSEBUTTONDOWN                              SDL_EVENT_MOUSE_BUTTON_DOWN
#define SDL_MOUSEBUTTONUP                                SDL_EVENT_MOUSE_BUTTON_UP
#define SDL_MouseIsHaptic                                SDL_IsMouseHaptic
#define SDL_MOUSEMOTION                                  SDL_EVENT_MOUSE_MOTION
#define SDL_MOUSEWHEEL                                   SDL_EVENT_MOUSE_WHEEL
#define SDL_mutex                                        SDL_Mutex
#define SDL_NewAudioStream                               SDL_CreateAudioStream
#define SDL_NUM_LOG_PRIORITIES                           SDL_LOG_PRIORITY_COUNT
#define SDL_NUM_SCANCODES                                SDL_SCANCODE_COUNT
#define SDL_NUM_SYSTEM_CURSORS                           SDL_SYSTEM_CURSOR_COUNT
#define SDL_OnApplicationDidBecomeActive                 SDL_OnApplicationDidEnterForeground
#define SDL_OnApplicationWillResignActive                SDL_OnApplicationWillEnterBackground
#define SDL_PATCHLEVEL                                   SDL_MICRO_VERSION
#define SDL_PIXELFORMAT_BGR444                           SDL_PIXELFORMAT_XBGR4444
#define SDL_PIXELFORMAT_BGR555                           SDL_PIXELFORMAT_XBGR1555
#define SDL_PIXELFORMAT_BGR888                           SDL_PIXELFORMAT_XBGR8888
#define SDL_PIXELFORMAT_RGB444                           SDL_PIXELFORMAT_XRGB4444
#define SDL_PIXELFORMAT_RGB555                           SDL_PIXELFORMAT_XRGB1555
#define SDL_PIXELFORMAT_RGB888                           SDL_PIXELFORMAT_XRGB8888
#define SDL_PixelFormatEnumToMasks                       SDL_GetMasksForPixelFormat
#define SDL_PointInFRect                                 SDL_PointInRectFloat
#define SDL_POLLSENTINEL                                 SDL_EVENT_POLL_SENTINEL
#define SDL_PREALLOC                                     SDL_SURFACE_PREALLOCATED
#define SDL_QUIT                                         SDL_EVENT_QUIT
#define SDL_ReadBE16                                     SDL_ReadU16BE
#define SDL_ReadBE32                                     SDL_ReadU32BE
#define SDL_ReadBE64                                     SDL_ReadU64BE
#define SDL_ReadLE16                                     SDL_ReadU16LE
#define SDL_ReadLE32                                     SDL_ReadU32LE
#define SDL_ReadLE64                                     SDL_ReadU64LE
#define SDL_RectEquals                                   SDL_RectsEqual
#define SDL_RENDER_DEVICE_RESET                          SDL_EVENT_RENDER_DEVICE_RESET
#define SDL_RENDER_TARGETS_RESET                         SDL_EVENT_RENDER_TARGETS_RESET
#define SDL_RenderCopy                                   SDL_RenderTexture
#define SDL_RenderCopyEx                                 SDL_RenderTextureRotated
#define SDL_RenderCopyExF                                SDL_RenderTextureRotated
#define SDL_RenderCopyF                                  SDL_RenderTexture
#define SDL_RenderDrawLine                               SDL_RenderLine
#define SDL_RenderDrawLineF                              SDL_RenderLine
#define SDL_RenderDrawLines                              SDL_RenderLines
#define SDL_RenderDrawLinesF                             SDL_RenderLines
#define SDL_RenderDrawPoint                              SDL_RenderPoint
#define SDL_RenderDrawPointF                             SDL_RenderPoint
#define SDL_RenderDrawPoints                             SDL_RenderPoints
#define SDL_RenderDrawPointsF                            SDL_RenderPoints
#define SDL_RenderDrawRect                               SDL_RenderRect
#define SDL_RenderDrawRectF                              SDL_RenderRect
#define SDL_RenderDrawRects                              SDL_RenderRects
#define SDL_RenderDrawRectsF                             SDL_RenderRects
#define SDL_RendererFlip                                 SDL_FlipMode
#define SDL_RenderFillRectF                              SDL_RenderFillRect
#define SDL_RenderFillRectsF                             SDL_RenderFillRects
#define SDL_RenderFlush                                  SDL_FlushRenderer
#define SDL_RenderGetClipRect                            SDL_GetRenderClipRect
#define SDL_RenderGetLogicalSize                         SDL_GetRenderLogicalPresentation
#define SDL_RenderGetMetalCommandEncoder                 SDL_GetRenderMetalCommandEncoder
#define SDL_RenderGetMetalLayer                          SDL_GetRenderMetalLayer
#define SDL_RenderGetScale                               SDL_GetRenderScale
#define SDL_RenderGetViewport                            SDL_GetRenderViewport
#define SDL_RenderGetWindow                              SDL_GetRenderWindow
#define SDL_RenderIsClipEnabled                          SDL_RenderClipEnabled
#define SDL_RenderLogicalToWindow                        SDL_RenderCoordinatesToWindow
#define SDL_RenderSetClipRect                            SDL_SetRenderClipRect
#define SDL_RenderSetLogicalSize                         SDL_SetRenderLogicalPresentation
#define SDL_RenderSetScale                               SDL_SetRenderScale
#define SDL_RenderSetViewport                            SDL_SetRenderViewport
#define SDL_RenderSetVSync                               SDL_SetRenderVSync
#define SDL_RenderWindowToLogical                        SDL_RenderCoordinatesFromWindow
#define SDL_RWclose                                      SDL_CloseIO
#define SDL_RWFromConstMem                               SDL_IOFromConstMem
#define SDL_RWFromFile                                   SDL_IOFromFile
#define SDL_RWFromMem                                    SDL_IOFromMem
#define SDL_RWops                                        SDL_IOStream
#define SDL_RWread                                       SDL_ReadIO
#define SDL_RWseek                                       SDL_SeekIO
#define SDL_RWsize                                       SDL_GetIOSize
#define SDL_RWtell                                       SDL_TellIO
#define SDL_RWwrite                                      SDL_WriteIO
#define SDL_SaveBMP_RW                                   SDL_SaveBMP_IO
#define SDL_ScaleModeLinear                              SDL_SCALEMODE_LINEAR
#define SDL_ScaleModeNearest                             SDL_SCALEMODE_NEAREST
#define SDL_sem                                          SDL_Semaphore
#define SDL_SemPost                                      SDL_SignalSemaphore
#define SDL_SemTryWait                                   SDL_TryWaitSemaphore
#define SDL_SemValue                                     SDL_GetSemaphoreValue
#define SDL_SemWait                                      SDL_WaitSemaphore
#define SDL_SemWaitTimeout                               SDL_WaitSemaphoreTimeout
#define SDL_SensorClose                                  SDL_CloseSensor
#define SDL_SensorFromInstanceID                         SDL_GetSensorFromID
#define SDL_SensorGetData                                SDL_GetSensorData
#define SDL_SensorGetInstanceID                          SDL_GetSensorID
#define SDL_SensorGetName                                SDL_GetSensorName
#define SDL_SensorGetNonPortableType                     SDL_GetSensorNonPortableType
#define SDL_SensorGetType                                SDL_GetSensorType
#define SDL_SensorOpen                                   SDL_OpenSensor
#define SDL_SENSORUPDATE                                 SDL_EVENT_SENSOR_UPDATE
#define SDL_SensorUpdate                                 SDL_UpdateSensors
#define SDL_SetClipRect                                  SDL_SetSurfaceClipRect
#define SDL_SetColorKey                                  SDL_SetSurfaceColorKey
#define SDL_SetThreadPriority                            SDL_SetCurrentThreadPriority
#define SDL_SetWindowDisplayMode                         SDL_SetWindowFullscreenMode
#define SDL_SIMD_ALIGNED                                 SDL_SURFACE_SIMD_ALIGNED
#define SDL_SIMDGetAlignment                             SDL_GetSIMDAlignment
#define SDL_size_add_overflow                            SDL_size_add_check_overflow
#define SDL_size_mul_overflow                            SDL_size_mul_check_overflow
#define SDL_strtokr                                      SDL_strtok_r
#define SDL_SwapBE16                                     SDL_Swap16BE
#define SDL_SwapBE32                                     SDL_Swap32BE
#define SDL_SwapBE64                                     SDL_Swap64BE
#define SDL_SwapLE16                                     SDL_Swap16LE
#define SDL_SwapLE32                                     SDL_Swap32LE
#define SDL_SwapLE64                                     SDL_Swap64LE
#define SDL_SYSTEM_CURSOR_ARROW                          SDL_SYSTEM_CURSOR_DEFAULT
#define SDL_SYSTEM_CURSOR_HAND                           SDL_SYSTEM_CURSOR_POINTER
#define SDL_SYSTEM_CURSOR_IBEAM                          SDL_SYSTEM_CURSOR_TEXT
#define SDL_SYSTEM_CURSOR_NO                             SDL_SYSTEM_CURSOR_NOT_ALLOWED
#define SDL_SYSTEM_CURSOR_SIZEALL                        SDL_SYSTEM_CURSOR_MOVE
#define SDL_SYSTEM_CURSOR_SIZENESW                       SDL_SYSTEM_CURSOR_NESW_RESIZE
#define SDL_SYSTEM_CURSOR_SIZENS                         SDL_SYSTEM_CURSOR_NS_RESIZE
#define SDL_SYSTEM_CURSOR_SIZENWSE                       SDL_SYSTEM_CURSOR_NWSE_RESIZE
#define SDL_SYSTEM_CURSOR_SIZEWE                         SDL_SYSTEM_CURSOR_EW_RESIZE
#define SDL_SYSTEM_CURSOR_WAITARROW                      SDL_SYSTEM_CURSOR_PROGRESS
#define SDL_SYSTEM_CURSOR_WINDOW_BOTTOM                  SDL_SYSTEM_CURSOR_S_RESIZE
#define SDL_SYSTEM_CURSOR_WINDOW_BOTTOMLEFT              SDL_SYSTEM_CURSOR_SW_RESIZE
#define SDL_SYSTEM_CURSOR_WINDOW_BOTTOMRIGHT             SDL_SYSTEM_CURSOR_SE_RESIZE
#define SDL_SYSTEM_CURSOR_WINDOW_LEFT                    SDL_SYSTEM_CURSOR_W_RESIZE
#define SDL_SYSTEM_CURSOR_WINDOW_RIGHT                   SDL_SYSTEM_CURSOR_E_RESIZE
#define SDL_SYSTEM_CURSOR_WINDOW_TOP                     SDL_SYSTEM_CURSOR_N_RESIZE
#define SDL_SYSTEM_CURSOR_WINDOW_TOPLEFT                 SDL_SYSTEM_CURSOR_NW_RESIZE
#define SDL_SYSTEM_CURSOR_WINDOW_TOPRIGHT                SDL_SYSTEM_CURSOR_NE_RESIZE
#define SDL_TABLESIZE                                    SDL_arraysize
#define SDL_TEXTEDITING                                  SDL_EVENT_TEXT_EDITING
#define SDL_TEXTEDITING_EXT                              SDL_EVENT_TEXT_EDITING_EXT
#define SDL_TEXTINPUT                                    SDL_EVENT_TEXT_INPUT
#define SDL_threadID                                     SDL_ThreadID
#define SDL_TLSCleanup                                   SDL_CleanupTLS
#define SDL_TLSGet                                       SDL_GetTLS
#define SDL_TLSSet                                       SDL_SetTLS
#define SDL_TRUE                                         true
#define SDL_UnionFRect                                   SDL_GetRectUnionFloat
#define SDL_UnionRect                                    SDL_GetRectUnion
#define SDL_UpperBlit                                    SDL_BlitSurface
#define SDL_UpperBlitScaled                              SDL_BlitSurfaceScaled
#define SDL_USEREVENT                                    SDL_EVENT_USER
#define SDL_WINDOW_ALLOW_HIGHDPI                         SDL_WINDOW_HIGH_PIXEL_DENSITY
#define SDL_WINDOW_FULLSCREEN_DESKTOP                    SDL_WINDOW_FULLSCREEN
#define SDL_WINDOW_INPUT_GRABBED                         SDL_WINDOW_MOUSE_GRABBED
#define SDL_WINDOW_SKIP_TASKBAR                          SDL_WINDOW_UTILITY
#define SDL_WINDOWEVENT_CLOSE                            SDL_EVENT_WINDOW_CLOSE_REQUESTED
#define SDL_WINDOWEVENT_DISPLAY_CHANGED                  SDL_EVENT_WINDOW_DISPLAY_CHANGED
#define SDL_WINDOWEVENT_ENTER                            SDL_EVENT_WINDOW_MOUSE_ENTER
#define SDL_WINDOWEVENT_EXPOSED                          SDL_EVENT_WINDOW_EXPOSED
#define SDL_WINDOWEVENT_FOCUS_GAINED                     SDL_EVENT_WINDOW_FOCUS_GAINED
#define SDL_WINDOWEVENT_FOCUS_LOST                       SDL_EVENT_WINDOW_FOCUS_LOST
#define SDL_WINDOWEVENT_HIDDEN                           SDL_EVENT_WINDOW_HIDDEN
#define SDL_WINDOWEVENT_HIT_TEST                         SDL_EVENT_WINDOW_HIT_TEST
#define SDL_WINDOWEVENT_ICCPROF_CHANGED                  SDL_EVENT_WINDOW_ICCPROF_CHANGED
#define SDL_WINDOWEVENT_LEAVE                            SDL_EVENT_WINDOW_MOUSE_LEAVE
#define SDL_WINDOWEVENT_MAXIMIZED                        SDL_EVENT_WINDOW_MAXIMIZED
#define SDL_WINDOWEVENT_MINIMIZED                        SDL_EVENT_WINDOW_MINIMIZED
#define SDL_WINDOWEVENT_MOVED                            SDL_EVENT_WINDOW_MOVED
#define SDL_WINDOWEVENT_RESIZED                          SDL_EVENT_WINDOW_RESIZED
#define SDL_WINDOWEVENT_RESTORED                         SDL_EVENT_WINDOW_RESTORED
#define SDL_WINDOWEVENT_SHOWN                            SDL_EVENT_WINDOW_SHOWN
#define SDL_WINDOWEVENT_SIZE_CHANGED                     SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED
#define SDL_WriteBE16                                    SDL_WriteU16BE
#define SDL_WriteBE32                                    SDL_WriteU32BE
#define SDL_WriteBE64                                    SDL_WriteU64BE
#define SDL_WriteLE16                                    SDL_WriteU16LE
#define SDL_WriteLE32                                    SDL_WriteU32LE
#define SDL_WriteLE64                                    SDL_WriteU64LE
#define SDLK_a                                           SDLK_A
#define SDLK_AUDIOFASTFORWARD                            SDLK_MEDIA_FAST_FORWARD
#define SDLK_AUDIOMUTE                                   SDLK_MUTE
#define SDLK_AUDIONEXT                                   SDLK_MEDIA_NEXT_TRACK
#define SDLK_AUDIOPLAY                                   SDLK_MEDIA_PLAY
#define SDLK_AUDIOPREV                                   SDLK_MEDIA_PREVIOUS_TRACK
#define SDLK_AUDIOREWIND                                 SDLK_MEDIA_REWIND
#define SDLK_AUDIOSTOP                                   SDLK_MEDIA_STOP
#define SDLK_b                                           SDLK_B
#define SDLK_BACKQUOTE                                   SDLK_GRAVE
#define SDLK_c                                           SDLK_C
#define SDLK_d                                           SDLK_D
#define SDLK_e                                           SDLK_E
#define SDLK_EJECT                                       SDLK_MEDIA_EJECT
#define SDLK_f                                           SDLK_F
#define SDLK_g                                           SDLK_G
#define SDLK_h                                           SDLK_H
#define SDLK_i                                           SDLK_I
#define SDLK_j                                           SDLK_J
#define SDLK_k                                           SDLK_K
#define SDLK_l                                           SDLK_L
#define SDLK_m                                           SDLK_M
#define SDLK_MEDIASELECT                                 SDLK_MEDIA_SELECT
#define SDLK_n                                           SDLK_N
#define SDLK_o                                           SDLK_O
#define SDLK_p                                           SDLK_P
#define SDLK_q                                           SDLK_Q
#define SDLK_QUOTE                                       SDLK_APOSTROPHE
#define SDLK_QUOTEDBL                                    SDLK_DBLAPOSTROPHE
#define SDLK_r                                           SDLK_R
#define SDLK_s                                           SDLK_S
#define SDLK_t                                           SDLK_T
#define SDLK_u                                           SDLK_U
#define SDLK_v                                           SDLK_V
#define SDLK_w                                           SDLK_W
#define SDLK_x                                           SDLK_X
#define SDLK_y                                           SDLK_Y
#define SDLK_z                                           SDLK_Z

//
// Surfaces. The pixel format is now an enum and not a pointer to a struct.
//
#define SDL_SURFACE_BYTES_PER_PIXEL(_s_) SDL_BYTESPERPIXEL((_s_)->format)
#define SDL_SURFACE_FORMAT(_s_)          ((_s_)->format)

//
// SDL3 has no SDL_Keysym, it puts the fields directly in the key event. The game passes these around
// a lot, so keep the type.
//
struct SDL_Keysym {
  SDL_Scancode scancode;
  SDL_Keycode  sym;
  SDL_Keymod   mod;
  uint16_t     unused;
};

static inline auto sdl_compat_keysym_from_event(const SDL_Event *e) -> SDL_Keysym
{
  SDL_Keysym k {};
  k.scancode = e->key.scancode;
  k.sym      = e->key.key;
  k.mod      = e->key.mod;
  return k;
}

static inline void sdl_compat_event_set_keysym(SDL_Event *e, const SDL_Keysym *k)
{
  e->key.scancode = k->scancode;
  e->key.key      = k->sym;
  e->key.mod      = k->mod;
}

//
// SDL_Event filter callbacks return bool now.
//
using sdl_compat_event_filter_ret_t = bool;

static inline auto sdl_compat_init(uint32_t flags) -> int { return SDL_Init(flags) ? 0 : -1; }

static inline auto sdl_compat_gl_make_current(SDL_Window *window, SDL_GLContext context) -> int
{
  return SDL_GL_MakeCurrent(window, context) ? 0 : -1;
}

static inline void sdl_compat_show_cursor(bool show)
{
  if (show) {
    SDL_ShowCursor();
  } else {
    SDL_HideCursor();
  }
}

//
// Mouse coordinates are floats now.
//
static inline auto sdl_compat_get_mouse_state(int *x, int *y) -> uint32_t
{
  float fx = 0;
  float fy = 0;

  auto buttons = SDL_GetMouseState(&fx, &fy);
  if (x != nullptr) {
    *x = static_cast< int >(fx);
  }
  if (y != nullptr) {
    *y = static_cast< int >(fy);
  }
  return static_cast< uint32_t >(buttons);
}

//
// SDL3 returns 64 bits; keep the 32 bit behaviour of SDL2.
//
static inline auto sdl_compat_get_ticks() -> uint32_t { return static_cast< uint32_t >(SDL_GetTicks()); }

//
// SDL3 returns an array of bool; this is the same layout as the uint8_t array of SDL2.
//
static inline auto sdl_compat_keyboard_state() -> const uint8_t * { return reinterpret_cast< const uint8_t * >(SDL_GetKeyboardState(nullptr)); }

//
// Display modes. Display index 0 in SDL2 becomes the primary display in SDL3.
//
static inline auto sdl_compat_num_display_modes() -> int
{
  int               count = 0;
  SDL_DisplayMode **modes = SDL_GetFullscreenDisplayModes(SDL_GetPrimaryDisplay(), &count);
  if (modes == nullptr) {
    return 0;
  }
  SDL_free(modes);
  return count;
}

static inline auto sdl_compat_get_display_mode(int index, SDL_DisplayMode *out) -> int
{
  int               count = 0;
  SDL_DisplayMode **modes = SDL_GetFullscreenDisplayModes(SDL_GetPrimaryDisplay(), &count);
  if (modes == nullptr) {
    return -1;
  }
  int ret = -1;
  if ((index >= 0) && (index < count)) {
    *out = *modes[ index ];
    ret  = 0;
  }
  SDL_free(modes);
  return ret;
}

static inline auto sdl_compat_get_current_display_mode(SDL_DisplayMode *out) -> int
{
  const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
  if (mode == nullptr) {
    return -1;
  }
  *out = *mode;
  return 0;
}

//
// SDL3 windows have no initial position argument; they are centered by default.
//
static inline auto sdl_compat_create_window(const char *title, int w, int h, uint64_t flags) -> SDL_Window *
{
  return SDL_CreateWindow(title, w, h, flags);
}

//
// SDL2 took the SDL_WINDOW_FULLSCREEN or SDL_WINDOW_FULLSCREEN_DESKTOP flag, or zero. SDL3 takes a bool and
// both of the SDL2 flags are the same thing; fullscreen at the desktop resolution.
//
static inline void sdl_compat_set_window_fullscreen(SDL_Window *window, uint64_t flags) { SDL_SetWindowFullscreen(window, flags != 0); }

static inline auto sdl_compat_create_rgb_surface(int w, int h, int bpp, uint32_t rmask, uint32_t gmask, uint32_t bmask, uint32_t amask)
    -> SDL_Surface *
{
  SDL_PixelFormat format = SDL_GetPixelFormatForMasks(bpp, rmask, gmask, bmask, amask);
  if (format == SDL_PIXELFORMAT_UNKNOWN) {
    //
    // SDL2 chose a default for 32 bits with no masks, that is, no alpha.
    //
    format = (bpp == 32) ? SDL_PIXELFORMAT_XRGB8888 : SDL_PIXELFORMAT_UNKNOWN;
  }
  return SDL_CreateSurface(w, h, format);
}

static inline auto sdl_compat_convert_surface(SDL_Surface *surface, SDL_PixelFormat format) -> SDL_Surface *
{
  return SDL_ConvertSurface(surface, format);
}

#else // SDL2

#include <SDL.h>
#include <SDL_mixer.h>

#define SDL_SURFACE_BYTES_PER_PIXEL(_s_) ((_s_)->format->BytesPerPixel)
#define SDL_SURFACE_FORMAT(_s_)          ((_s_)->format->format)

static inline auto sdl_compat_keysym_from_event(const SDL_Event *e) -> SDL_Keysym { return e->key.keysym; }

static inline void sdl_compat_event_set_keysym(SDL_Event *e, const SDL_Keysym *k) { e->key.keysym = *k; }

using sdl_compat_event_filter_ret_t = int;

static inline auto sdl_compat_init(uint32_t flags) -> int { return SDL_Init(flags); }

static inline auto sdl_compat_gl_make_current(SDL_Window *window, SDL_GLContext context) -> int { return SDL_GL_MakeCurrent(window, context); }

static inline void sdl_compat_show_cursor(bool show) { SDL_ShowCursor(show ? 1 : 0); }

static inline auto sdl_compat_get_mouse_state(int *x, int *y) -> uint32_t { return SDL_GetMouseState(x, y); }

static inline auto sdl_compat_get_ticks() -> uint32_t { return SDL_GetTicks(); }

static inline auto sdl_compat_keyboard_state() -> const uint8_t * { return SDL_GetKeyboardState(nullptr); }

static inline auto sdl_compat_num_display_modes() -> int { return SDL_GetNumDisplayModes(0); }

static inline auto sdl_compat_get_display_mode(int index, SDL_DisplayMode *out) -> int { return SDL_GetDisplayMode(0, index, out); }

static inline auto sdl_compat_get_current_display_mode(SDL_DisplayMode *out) -> int { return SDL_GetCurrentDisplayMode(0, out); }

static inline auto sdl_compat_create_window(const char *title, int w, int h, uint64_t flags) -> SDL_Window *
{
  return SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, static_cast< uint32_t >(flags));
}

static inline void sdl_compat_set_window_fullscreen(SDL_Window *window, uint64_t flags)
{
  SDL_SetWindowFullscreen(window, static_cast< uint32_t >(flags));
}

static inline auto sdl_compat_create_rgb_surface(int w, int h, int bpp, uint32_t rmask, uint32_t gmask, uint32_t bmask, uint32_t amask)
    -> SDL_Surface *
{
  return SDL_CreateRGBSurface(0, w, h, bpp, rmask, gmask, bmask, amask);
}

static inline auto sdl_compat_convert_surface(SDL_Surface *surface, uint32_t format) -> SDL_Surface *
{
  return SDL_ConvertSurfaceFormat(surface, format, 0);
}

#endif // USE_SDL3

#endif
