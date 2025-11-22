/**
 * VID6608 gauge motor driver library
 * by Petr Golovachev <petro@petro.ws>, 2025
 *
 * https://github.com/petrows/arduino-vid6608
 *
 * Licensed under the GPL, see LICENSE.txt for details.
 */

#ifndef vid6608_h
#define vid6608_h

#include <Arduino.h>

/**
 * @brief Default max steps for VID6608 motor + x27.168 clones
 * @note driver has 12 steps per degree normally
 * this value is default only, can be changed as argument, @see vid6608::vid6608()
 */
#define VID6608_DEFAULT_MAX_STEPS 320 * 12
/**
 * @brief Default speed while homing, in microseconds
 * this value is default only, can be changed as argument, @see vid6608::zero()
 */
#define VID6608_DEFAULT_ZERO_SPEED 600

/**
 * @brief Driver object class
 *
 * See example usage in: examples/RandomMove/RandomMove.cpp
 *
 */
class vid6608 {
  public:
    /**
     * @brief Object constructor, specify two control pins and overall motor capacity here
     *
     * @param stepPin pin connected to f(scx)N chip input, controls movement
     * @param dirPin pin connected to CW/CCW(N) chip input, controls movement direction
     * @param maxSteps full capacity in steps for motor. Note that VID6606 and its clones provides 12 steps per degree. Default capacity is defined in VID6608_DEFAULT_MAX_STEPS (320 degrees * 12 steps)
     */
    vid6608(int stepPin, int dirPin, uint16_t maxSteps = VID6608_DEFAULT_MAX_STEPS);

    /**
     * @brief Resets zero position to actual 0 position
     *
     * Uses optimized method to reduce bouncing end-stops
     * @warning this function is blocking, execution is delayed upon done
     *
     * @param delay single step delay, controls sthe speed of motor, default is deined in VID6608_DEFAULT_ZERO_SPEED
     */
    void zero(uint16_t delay = VID6608_DEFAULT_ZERO_SPEED);

    /**
     * @brief Shedules movement to defined absolute position
     *
     * Input is checked for sanity: must be in range 0...maxSteps-1. Values bigger are threated as maxSteps-1.
     * @warning this function is asynchronous, actual movement is done in the loop() function.
     * @warning next move will be scheduled after current move is done to avoid drive jittering.
     *
     * @param position absolute position in range 0...maxSteps-1
     */
    void moveTo(uint16_t position);

    /**
     * @brief Test if motor is moving
     *
     * Return true, if drive still have sheduled steps (that means that next loop() call will result impulse).
     *
     * @return true if drive is moveemnt
     * @return false if drive is stopped
     */
    bool isMoving();

    /**
     * @brief Test if motor is stopped
     *
     * @return true if drive is stopped
     * @return false if drive is moveemnt
     */
    bool isStopped() { return !isMoving(); }

    /**
     * @brief Returns current real absolute position
     *
     * @return uint16_t current real drive position in steps
     */
    uint16_t getPosition();

    /**
     * @brief Main loop function
     *
     * This function must be called in main loop to manage actual movement
     * and proper signal generation.
     * @warning: this function is asynchronous, but may delay up to single step (depends on accelTable, up to 3ms using default one)
     */
    void loop();

    /**
     * @brief Acceleration Table struct
     * This struct holds the acceleration curve, to slow down movement close to start/end positions.
     */
    typedef struct {
      uint16_t distance;  /** in steps, from start or to end */
      uint16_t delay;     /** in microseconds */
    } AccelTable;
    /**
     * @brief Set the Acceleration Table object
     *
     * This function sets the acceleration curve, to slow down movement close to start/end positions.
     * Inspired by: https://github.com/clearwater/SwitecX25/pull/31
     * See ``defaultAccelTable`` defenition for example.
     *
     * @param table acceleration table, must be array of @struct AccelTable
     */
    template <typename T, size_t N> void setAccelTable(T (&table)[N]) {
      accelTable = table;
      accelTableSize = N;
      accelMaxDistance = accelTable[accelTableSize - 1].distance;
      accelMaxDelay = accelTable[accelTableSize - 1].delay;
    };

  private:
    enum MoveState {
      STATE_IDLE = 0,
      STATE_MOVING = 1,
    };
    enum MoveDirection {
      MOVE_NONE = -1,
      MOVE_FORWARD = 0,
      MOVE_BACKWARD = 1,
    };
    uint16_t maxSteps;              // Maximum steps for the motor
    uint16_t currentPosition;       // Current position in steps
    uint16_t targetPosition;        // Target position in steps
    uint16_t targetPositionNext;    // Target position in steps (scheduled for next move)
    uint16_t moveLeft;              // How long we have to move left
    uint16_t moveDone;              // How long we have traveled
    int stepPin;                    // Pin connected to step input
    int dirPin;                     // Pin connected to direction input
    MoveState moveState;            // Current move state
    MoveDirection moveDirection;    // Current move state
    MoveDirection dirPinState;      // Direction pin last state, tri-state to force update on first step
    AccelTable *accelTable;         // Accel table can be modified
    uint16_t accelTableSize;        // How many rows in the acceleration table
    uint16_t accelMaxDistance;      // Max distance from the acceleration table
    uint16_t accelMaxDelay;         // Max delay from the acceleration table
    // Actual move
    void step(MoveDirection direction, uint16_t delay);
    // Get current acceleration speed (delay)
    uint16_t getDelay(uint16_t distance);
};

#endif // vid6608_h
