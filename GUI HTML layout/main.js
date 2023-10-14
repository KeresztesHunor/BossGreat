$(() => {
    const SAMPLE_SELECTION_BUTTONS = $("#sample-selection-buttons > div").toArray();
    $(SAMPLE_SELECTION_BUTTONS[0]).addClass("active");
    SAMPLE_SELECTION_BUTTONS.forEach((button, index) => {
        $(button).on("click", () => {
            let removedActiveClass = false;
            let i = 0;
            while (i < SAMPLE_SELECTION_BUTTONS.length && !removedActiveClass)
            {
                const SAMPLE_SELECTION_BUTTON = $(SAMPLE_SELECTION_BUTTONS[i++])
                if (SAMPLE_SELECTION_BUTTON.hasClass("active"))
                {
                    SAMPLE_SELECTION_BUTTON.removeClass("active");
                    removedActiveClass = true;
                }
            }
            $(SAMPLE_SELECTION_BUTTONS[index]).addClass("active");
        });
    });
    const TOGGLE_RECORD_MODE_BUTTON = $("#toggle-record-mode-button");
    TOGGLE_RECORD_MODE_BUTTON.on("click", () => {
        TOGGLE_RECORD_MODE_BUTTON.toggleClass("active");
    });
});