function Ball(x, y, velocity, acceleration, color) {
    this.x = x;
    this.y = y;
    this.radius = 50;
    this.velocity = velocity;
    this.acceleration = acceleration;
    this.color = color;

    this.sprite = new Path.Circle({
        center: [x, y],
        radius: this.radius,
        fillColor: color
    });

    this.update = function (deltatime) {
        this.velocity += this.acceleration * deltatime;
        this.x += this.velocity * deltatime;
        this.sprite.position = [this.x, this.y];
    };
}

function collision(ball1, ball2) {
    return ball1.sprite.intersects(ball2.sprite);
};

var ball = new Ball(0, 300, 0, 1000, "cyan");
var ball2 = new Ball(800, 300, 0, -1000, "red");

var dt = 1.0 / 60;

setInterval(function () {
    ball.update(dt);
    ball2.update(dt);

    if (collision(ball, ball2)) {
        ball.velocity = -ball.velocity;
        ball.acceleration = -ball.acceleration;
        ball2.velocity = -ball2.velocity;
        ball2.acceleration = -ball2.acceleration;
    }

}, 1000 * dt);
