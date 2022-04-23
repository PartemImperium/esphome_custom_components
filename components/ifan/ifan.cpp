#include "ifan.h"
#include "esphome/components/fan/fan_helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace ifan {

static const char *const TAG = "ifan.fan";

void IFan::setup() {
  auto restore = this->restore_state_();
  if (restore.has_value()) {
    restore->apply(*this);
    this->write_state_();
  }
}
void IFan::dump_config() { LOG_FAN("", "I Fan", this); }
fan::FanTraits IFan::get_traits() {
  return fan::FanTraits(true, this->direction_ != nullptr, this->speed_count_);
}
void IFan::control(const fan::FanCall &call) {
  if (call.get_state().has_value())
    this->state = *call.get_state();
  if (call.get_speed().has_value())
    this->speed = *call.get_speed();
  if (call.get_direction().has_value())
    this->direction = *call.get_direction();

  this->write_state_();
  this->publish_state();
}
void IFan::write_state_() {
  float speed = this->state ? static_cast<float>(this->speed) / static_cast<float>(this->speed_count_) : 0.0f;
  this->output_->set_level(speed);

  if (this->direction_ != nullptr)
    this->direction_->set_state(this->direction == fan::FanDirection::REVERSE);
}

}  // namespace speed
}  // namespace esphome