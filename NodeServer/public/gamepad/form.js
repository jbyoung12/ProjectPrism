$(function() {
  var contactForm = $('#contact-form');
  var formMessage = $('#messages');
  var contactButtons = $('#contact-form button');
  var submitButton = $('#contact-form button:submit');

  function resetButtons() {
    $(contactButtons).prop('disabled', false);
    $(submitButton).html('Submit');
  }

  $(contactForm).submit(function(event) {
    event.preventDefault();
    var formData = $(contactForm).serialize();
    $(formMessage).empty();
    $(formMessage).removeClass();
    $(contactButtons).prop('disabled', true);
    $(submitButton).html('Submitting');
    $.ajax({
      type: 'POST',
      url: $(contactForm).attr('action'),
      data: formData
    }).done(function(response) {
      $(formMessage).addClass('success');
      setTimeout(function() {
        $(formMessage).empty()
      }, 5000);
      $(formMessage).append("<p>" + response.success + "</p>");
      grecaptcha.reset();
      $(contactForm).trigger("reset");
      resetButtons();
    }).fail(function(response) {
      var list = $(formMessage).append('<ul></ul>').find('ul');
      $.each(response.responseJSON.message, function() {
        $(list).append("<li>" + this + "</li>");
      });
      grecaptcha.reset();
      resetButtons();
    })
  });
});
